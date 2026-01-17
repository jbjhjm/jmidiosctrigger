/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Command.h"
#include "StatusLog.h"
#include "VarHandler.h"
#include "../libraries/pugixml.hpp" 

//==============================================================================



struct LookupMapKeyComparator
{
    int compareElements(const int a, const int b) const
    {
        return a - b;
    }
};

juce::String oscParamsToString(const juce::Array<OscParam> & oscParams);
juce::String oscInstructionToString(const Command& instruction);

/**
*/
class XMLParser
{
public:
    //==============================================================================
    static XMLParser& getInstance();
    XMLParser();
    ~XMLParser();

    bool loadXmlData(pugi::xml_document& doc);
    juce::String generateXmlDocumentation();
	
	const Command findCachedMapping(int channel, int note);
    int countNodeChildren(pugi::xml_node& node, const char * name);
	
	
    bool xmlReadyState = false;
	
	
private:
    //==============================================================================
	void loadXmlConfigurationData(pugi::xml_node configNode);
	void cacheXmlMappings(pugi::xml_node mappingsNode);
    StatusLog logger;// = StatusLog::getInstance();
	juce::HashMap<int, Command> lookupMap;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(XMLParser)
};
