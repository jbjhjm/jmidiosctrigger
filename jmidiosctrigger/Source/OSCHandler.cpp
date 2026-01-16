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
	
	if(configState.hasProperty(CONFIGPROPS::XML_IP)) {
		ipVar = &configState.getProperty(CONFIGPROPS::XML_IP);
		if(ipVar->isString()) {
			ip = ipVar->toString();
		}
	}
	if(configState.hasProperty(CONFIGPROPS::IP)) {
		// TODO: Not working yet
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

	if(!isValidIPv4(ip.getCharPointer())) {
		logger.log("Invalid IP detected: " + ip);
		return false;
	}
	if(!isValidPort(port)) {
		logger.log("Invalid port detected: " + juce::String(port));
		return false;
	}

	logger.log("Connecting to " + ip + ":" + juce::String(port) + "...");
	if (!sender.connect (ip, port)) { // [4]
		showConnectionErrorMessage ("Error: could not connect to UDP port 9001.");
		return false;
	}
	return true;
}

bool OSCHandler::disconnect() {
	return sender.disconnect();
}

bool OSCHandler::sendOSC(const pugi::xml_node& xmlOSCNode, juce::MidiMessage& midiInput) {
	juce::String command = juce::String (xmlOSCNode.attribute("command").as_string());
    int velocity = midiInput.getVelocity();
	float percentage = velocity / 255.0;
	
	logger.log(">>> Matched NoteOn [channel=" + juce::String(midiInput.getChannel()) +"]"
		+ " [key=" + juce::String(midiInput.getNoteNumber())+"]"
		+ " --> OSC: " + command
	, 0);

	juce::OSCMessage msg("/juce/rotaryknob");
	msg.addFloat32(percentage);

	if (!sender.send (msg)) {
        showConnectionErrorMessage ("Error: could not send OSC message.");
		return false;
	}
	return true;
}