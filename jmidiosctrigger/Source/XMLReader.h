/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "StatusLog.h"
#include "XMLParser.h"
#include "../libraries/pugixml.hpp" 

//==============================================================================

/**
*/
class XMLReader 
{
public:
    //==============================================================================
    static XMLReader& getInstance();
    XMLReader();
    ~XMLReader();

    bool loadXmlFile(const juce::String& filePath);
    bool loadXmlFile(const juce::File& fi);
    bool abortLoadXmlFile();
    bool completeLoadXmlFile();
    bool reloadFile();
    bool isReady();

    StatusLog logger;// = StatusLog::getInstance();
    XMLParser* parser;

    juce::Value xmlFilePath;
    pugi::xml_document xmlDoc;
    juce::Value documentation;


private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (XMLReader)
};
