/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "XMLReader.h"
#include "FileUtils.h"

XMLReader& XMLReader::getInstance()
{
	static XMLReader instance;
	return instance;
}

//==============================================================================
XMLReader::XMLReader():
	logger (StatusLog::getInstance()),
	parser (&XMLParser::getInstance())
{
	documentation = "No config data loaded yet";
	xmlFilePath = "";
}

XMLReader::~XMLReader()
{
}

// overload: from juce::File
bool XMLReader::loadXmlFile(const juce::File& fi)
{
	if (!fi.exists())
	{
		return abortLoadXmlFile();
	}

	xmlFilePath.setValue(fi.getFullPathName());

	pugi::xml_document xmlDoc;
	pugi::xml_parse_result xmlReadSuccess = xmlDoc.load_file(fi.getFullPathName().toRawUTF8());

	if (xmlReadSuccess == false) {
		logger.log("Error while reading file: " + juce::String(xmlReadSuccess.description()),1);
		return abortLoadXmlFile();
	}
	else 
	{
		logger.log("Successfully parsed file." ,1);
		if (!parser->loadXmlData(xmlDoc)) { 
			return abortLoadXmlFile();
		};

		documentation = parser->generateXmlDocumentation();
		return completeLoadXmlFile();
	}
	
}

// overload: from juce::String
bool XMLReader::loadXmlFile(const juce::String& filePath)
{
	const auto resolvedPath = FileUtils::solveRelativeFilePath(filePath);
	const auto& fi = juce::File(resolvedPath);

	logger.debug("load xml file " + resolvedPath, 1);
	
	return this->loadXmlFile(fi);
}


bool XMLReader::isReady() {
	if (!parser) return false;
	return parser->xmlReadyState;
}

bool XMLReader::abortLoadXmlFile()
{
	//xmlReadyState = false;
	return false;
}

bool XMLReader::completeLoadXmlFile()
{
	//xmlReadyState = true;
	return true;
}

bool XMLReader::reloadFile()
{
	const auto filePath = xmlFilePath.getValue().toString();
	if (filePath == "") {
		logger.log("No config file is selected, cannot reload.");
		return false;
	}
	logger.log("Reloading config file...");
	return this->loadXmlFile(xmlFilePath.toString());
}

