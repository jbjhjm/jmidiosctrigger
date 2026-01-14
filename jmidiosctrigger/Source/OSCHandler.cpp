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
	if (!sender.connect ("127.0.0.1", 9001)) { // [4]
		showConnectionErrorMessage ("Error: could not connect to UDP port 9001.");
		return false;
	}
	return true;
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