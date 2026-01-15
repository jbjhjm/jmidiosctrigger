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
class XMLParser
{
public:
    //==============================================================================
    static XMLParser& getInstance();
    XMLParser();
    ~XMLParser();

    bool loadXmlData(pugi::xml_document* doc);
    juce::String generateXmlDocumentation();

	pugi::xml_node findMappingNode(int channel, int note);
    int countNodeChildren(pugi::xml_node& node, const char * name);

	bool XMLParser::findEntryforMidiEvent(juce::MidiMessage &inputInfo, pugi::xml_node &xmlEntry);

    StatusLog logger;// = StatusLog::getInstance();

    pugi::xml_document* xmlDoc;
    pugi::xml_node xmlRootNode;
    pugi::xml_node xmlConfigNode;
    pugi::xml_node xmlMappingsNode;
    bool xmlReadyState = false;


private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(XMLParser)
};
