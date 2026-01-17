/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "StatusLog.h"
#include "Command.h"
#include "FileUtils.h"
#include "../libraries/pugixml.hpp" 

//==============================================================================


/**
*/
class VarHandler
{
public:
    //==============================================================================
    static VarHandler& getInstance();
    VarHandler();
    ~VarHandler();

	void readXmlVariables(pugi::xml_node configNode);
	void resetVariables();
	void resetDefaultVariables();
	const juce::var getVariable(juce::String name);
	void setVariable(juce::String name, float value);
	bool command(const Command& instruction, juce::MidiMessage& midiInput);

	
private:
    //==============================================================================
    StatusLog logger;// = StatusLog::getInstance();

	juce::HashMap<juce::String, juce::var> variableDefaults;
	juce::HashMap<juce::String, juce::var> variableAssignments;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VarHandler)
};
