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

struct OscParam {
	juce::String type;
	juce::String value;
};

struct OscInstruction {
	juce::String command;
	juce::Array<OscParam> params;
};

struct LookupMapKeyComparator
{
    int compareElements(const int a, const int b) const
    {
        return a - b;
    }
};

juce::String oscParamsToString(const juce::Array<OscParam> & oscParams);
juce::String oscInstructionToString(const OscInstruction& instruction);

/**
*/
class XMLParser
{
public:
    //==============================================================================
    static XMLParser& getInstance();
    XMLParser();
    ~XMLParser();

    bool loadXmlData(pugi::xml_document* doc);
	void loadXmlConfigurationData(pugi::xml_node configNode);
	void cacheXmlMappings(pugi::xml_node mappingsNode);
    juce::String generateXmlDocumentation();

	const OscInstruction findCachedMapping(int channel, int note);
	pugi::xml_node findMappingNode(int channel, int note);
    int countNodeChildren(pugi::xml_node& node, const char * name);

	bool XMLParser::findEntryforMidiEvent(juce::MidiMessage &inputInfo, pugi::xml_node &xmlEntry);

    StatusLog logger;// = StatusLog::getInstance();

    pugi::xml_document* xmlDoc;
    pugi::xml_node xmlMappingsNode;
    bool xmlReadyState = false;

	juce::HashMap<int, OscInstruction> lookupMap;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(XMLParser)
};
