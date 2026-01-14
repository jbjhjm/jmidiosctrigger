/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "StatusLog.h"
#include "../libraries/pugixml.hpp" 

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
    bool sendOSC(const pugi::xml_node& xmlOSCNode, juce::MidiMessage& midiInput);


	
private:
    //==============================================================================
    StatusLog logger;// = StatusLog::getInstance();
	juce::OSCSender sender;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCHandler)
};
