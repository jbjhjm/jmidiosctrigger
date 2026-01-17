/*
  ==============================================================================

    OSCHandler.cpp
    Created: 14 Jan 2026 8:07:08pm
    Author:  JDESKTOP

  ==============================================================================
*/

#include "OSCHandler.h"

void showConnectionErrorMessage (const juce::String& messageText)
{
    juce::AlertWindow::showMessageBoxAsync (juce::AlertWindow::WarningIcon,
        "Connection error",
        messageText,
        "OK");
}

bool isValidIPv4(const char *IPAddress)
{
   unsigned int a,b,c,d;
   if(!sscanf(IPAddress,"%d.%d.%d.%d", &a, &b, &c, &d) == 4) return false;
   return a <= 255 && b <= 255 && c <= 255 && d <= 255;
}
bool isValidPort(int port)
{
   return port > 0 && port <= 65535;
}

juce::String replaceVarsInString(const juce::String& input, float velocity)
{
	auto& varHandler = VarHandler::getInstance();
	auto& logger = StatusLog::getInstance();
	const auto stdString = input.toStdString();
	auto itr = stdString.begin();
	auto end = stdString.end();
	// StatusLog::getInstance().log("searching vars in: "+juce::String(stdString));

	std::regex pattern("\\$\\w+");
	juce::String out;

	for (std::smatch match; std::regex_search(itr, end, match, pattern); itr = match[0].second)
	{
		out += juce::String(match.prefix());
		const auto varName = juce::String(match.str()).substring(1);
		if(varName == "velocity") {
			out += juce::String(velocity);
		} else {
			const auto& var = varHandler.getVariable(varName);
			if(var.isDouble()) {
				out += juce::String((float)var);
			} else if(var.isString()) {
				out += var.toString();
			} else {
				logger.log("WARN: could not find replacement value for " + varName);
			}
		}
		itr += match.position() + match.length();
	}
	
	std::string rest(itr, end);
	out += juce::String(rest);

	// StatusLog::getInstance().log("replaced vars, result: "+out);

	return out;
}

OSCHandler& OSCHandler::getInstance()
{
	static OSCHandler instance;
	return instance;
}

//==============================================================================
OSCHandler::OSCHandler():
	logger (StatusLog::getInstance())
{
	DBG(">>>>>>>>>>>>>OSCHandler::OSCHandler");
}

OSCHandler::~OSCHandler()
{
}

bool OSCHandler::connect() {
	auto& configState = Store::getState(STATES::Config);
	const juce::var* ipVar = nullptr;
	juce::String ip ;
	int port;
	int fallbackPort = 0;
	
	if(configState.hasProperty(CONFIGPROPS::XML_IP)) {
		ipVar = &configState.getProperty(CONFIGPROPS::XML_IP);
		if(ipVar->isString()) {
			ip = ipVar->toString();
		}
	}
	if(configState.hasProperty(CONFIGPROPS::IP)) {
		ipVar = &configState.getProperty(CONFIGPROPS::IP);
		if(ipVar->isString() && ipVar->toString() != "" ) {
			ip = ipVar->toString();
		}
	}
	if(configState.hasProperty(CONFIGPROPS::XML_Port)) {
		ipVar = &configState.getProperty(CONFIGPROPS::XML_Port);
		if(ipVar->isInt()) {
			port = static_cast<int>(*ipVar);
		}
	}
	if(configState.hasProperty(CONFIGPROPS::Port)) {
		ipVar = &configState.getProperty(CONFIGPROPS::Port);
		if(ipVar->isString() && ipVar->toString() != "" ) {
			port = ipVar->toString().getIntValue();
		}
	}
	if(configState.hasProperty(CONFIGPROPS::XML_PortFallback)) {
		ipVar = &configState.getProperty(CONFIGPROPS::XML_PortFallback);
		if(ipVar->isInt()) {
			fallbackPort = static_cast<int>(*ipVar);
		}
	}

	if(!isValidIPv4(ip.getCharPointer())) {
		logger.log("Invalid IP detected: " + ip);
		return false;
	}
	if(!isValidPort(port)) {
		logger.log("Invalid port detected: " + juce::String(port));
		return false;
	}

	logger.log("Connecting to " + ip + ":" + juce::String(port) + "...");
	if (sender.connect (ip, port)) return true;

	logger.log("Error: Failed to connect.");

	if(isValidPort(fallbackPort)) {
		logger.log("Connecting to fallback " + ip + ":" + juce::String(fallbackPort) + "...");
		if (sender.connect (ip, fallbackPort)) return true;
		logger.log("Error: Failed to connect.");
	} else {
		logger.log("No valid fallbackPort found.");
	}

	showConnectionErrorMessage ("Error: could not establish OSC connection.");

	return false;
		
}

bool OSCHandler::disconnect() {
	return sender.disconnect();
}

bool OSCHandler::sendOSC(const Command& instruction, juce::MidiMessage& midiInput) {

	// standard OSC does not allow whitespaces in address and OSCSender checks for conforming address.
	// so we need to replace whitespaces and convert back in target application if needed.
	auto command = instruction.command.replaceCharacter(' ','~');

	float floatVelocity = midiInput.getFloatVelocity() * 100.0; // MA expects float in range of 0-100
	juce::OSCMessage msg(command);

	for (const auto& param : instruction.params) {
		// in case the param value is $xxx
		
		if(param.type == "f") {
			if(param.value.startsWithChar('$')) {
				if(param.value == "$velocity") {
					msg.addFloat32(floatVelocity * param.multiplier);
				} else {
					auto& val = VarHandler::getInstance().getVariable(param.value.substring(1));
					if(val.isDouble()) {
						msg.addFloat32((float) val * param.multiplier);
					} else {
						logger.log("Error: tried to assign a non-numeric variable to float osc param.");
					}
				}
			} else {
				msg.addFloat32(param.value.getFloatValue());
			}
		} else if (param.type == "s") {
			auto const parsed = replaceVarsInString(param.value, floatVelocity);
			logger.log("Parsed String: "+parsed);
			msg.addString(parsed);
		} else {
			logger.log("ERR: Unsupported param type " + param.type);
			return false;
		}
	}
	
	logger.log(">>> Matched NoteOn [channel=" + juce::String(midiInput.getChannel()) +"]"
		+ " [key=" + juce::String(midiInput.getNoteNumber())+"]"
		+ " --> OSC: " + command + oscParamsToString(instruction.params)
	, 0);

	if (!sender.send (msg)) {
        showConnectionErrorMessage ("Error: could not send OSC message.");
		return false;
	}
	return true;
}
