/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "XmlParser.h"
#include "StatusLog.h"
#include "Command.h"
#include "VarHandler.h"

//==============================================================================

/**
*/
class OSCHandler 
{
public:
    //==============================================================================
    static OSCHandler& getInstance();
    OSCHandler();
    ~OSCHandler();

	bool connect();
	bool disconnect();
    bool sendOSC(const Command& instruction, juce::MidiMessage& midiInput);


	
private:
    //==============================================================================
    StatusLog logger;// = StatusLog::getInstance();
	juce::OSCSender sender;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCHandler)
};
