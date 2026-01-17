/*
  ==============================================================================

    XMLParser.cpp
    Created: 27 Jun 2023 10:22:37am
    Author:  Jannik

  ==============================================================================
*/
/**
  Reminder on XML features
  - <mapping> listens to a certain kind of midi input (=targetNode)

  pugixml does not use raw pointers because it uses pugi::xml_node and pugi::xml_attribute as lightweight, non-owning handles (essentially smart pointer-like wrappers) to XML data stored within a pugi::xml_document.	
	This design means:
	xml_node objects are not raw pointers but handles to nodes in the document tree. 
	They are shallow references—they do not own the data they point to. 
	The actual XML data is owned and managed by the xml_document object.
	You must keep the xml_document alive as long as you have xml_node handles pointing into it. 
  */

#include "XMLParser.h"

// we know channel <= 16 and note <= 127, so we can join the ints into a combined int.
int getCacheKey(int channel, int note)
{
	return (channel << 8) + note;
}
juce::String cacheKeyToChannelAndKeyString(int cacheKey)
{
	return "[" + juce::String(cacheKey >> 8) + "." + juce::String(cacheKey & 0x000000FF) + "]";
}

juce::String oscParamsToString(const juce::Array<OscParam> & oscParams) {
	juce::String paramsString = " -- ";
	for (const auto& param : oscParams) {
		paramsString += "[" + param.type + "," + param.value + "] ";
	}
	return paramsString;
}

juce::String oscInstructionToString(const Command& instruction) {
	auto paramsString = oscParamsToString(instruction.params);
	return juce::String( instruction.command + paramsString);
}


XMLParser& XMLParser::getInstance()
{
	static XMLParser instance;
	return instance;
}

//==============================================================================
XMLParser::XMLParser():
	logger (StatusLog::getInstance())
{
	DBG(">>>>>>>>>>>>>XMLParser::XMLParser");
}

XMLParser::~XMLParser()
{
}


bool XMLParser::loadXmlData(pugi::xml_document& doc)
{
	xmlReadyState = false;
	lookupMap.clear();
	VarHandler::getInstance().resetVariables();
	VarHandler::getInstance().resetDefaultVariables();

	auto xmlRootNode = doc.document_element();
	if (!xmlRootNode) { DBG("Error: No XML root node found. "); return false; }
	DBG("Debug: Selected root node " + juce::String(xmlRootNode.name()));

	auto xmlConfigNode = xmlRootNode.child("config");
	if (!xmlConfigNode) { logger.log("Error: No XML <config> node found. "); }
	// logger.log("Found number of configs: " + juce::String(countNodeChildren(xmlConfigNode, "")));

	auto xmlVarsNode = xmlRootNode.child("values");
	if(xmlVarsNode) VarHandler::getInstance().readXmlVariables(xmlVarsNode);

	auto xmlMappingsNode = xmlRootNode.child("mappings");
	if (!xmlMappingsNode) { logger.log("Error: No XML <mappings> node found. "); return false; }
	// logger.log("Found number of mappings: " + juce::String(countNodeChildren(xmlMappingsNode, "mapping")));
	DBG("Debug: Selected mappings group node " + juce::String(xmlMappingsNode.name()));

	// for (pugi::xml_node child = xmlConfigNode.first_child(); child; child = child.next_sibling()) {
	// 	logger.log("xmlConfigNode child name: " + juce::String(child.name()) );
	// }

	loadXmlConfigurationData(xmlConfigNode);
	cacheXmlMappings(xmlMappingsNode);
	xmlReadyState = true;
	return true;
}

void XMLParser::loadXmlConfigurationData(pugi::xml_node configNode)
{
	auto& configState = Store::getState(STATES::Config);
	// auto x = countNodeChildren(configNode, "");
	// logger.log("loadXmlConfigurationData: <config> node contains " + juce::String(x) + " children");

	for (pugi::xml_node node = configNode.first_child(); node; node = node.next_sibling()) {
		auto const& name = node.name();
		if(strcmp(name, "IP") == 0) {
			configState.setProperty(CONFIGPROPS::XML_IP, juce::String( node.attribute("value").as_string() ) , nullptr);
			// logger.log("Read XML Config: XML_IP = " + juce::String( node.attribute("value").as_string() ));
		} else if(strcmp(name, "Port") == 0) {
			configState.setProperty(CONFIGPROPS::XML_Port, node.attribute("value").as_int() , nullptr );
			// logger.log("Read XML Config: Port = " + juce::String( node.attribute("value").as_int() ));
		} else if(strcmp(name, "PortFallback") == 0) {
			configState.setProperty(CONFIGPROPS::XML_PortFallback, node.attribute("value").as_int() , nullptr );
			// logger.log("Read XML Config: Port = " + juce::String( node.attribute("value").as_int() ));
		} else if(strcmp(name, "Filter_maxNote") == 0) {
			configState.setProperty(CONFIGPROPS::FilterMaxNote, node.attribute("value").as_int() , nullptr );
			// logger.log("Read XML Config: Filter_maxNote = " + juce::String( node.attribute("value").as_int() ));
		} else if(strcmp(name, "Filter_extraChannel") == 0) {
			configState.setProperty(CONFIGPROPS::FilterExtraChannel, node.attribute("value").as_int() , nullptr );
			// logger.log("Read XML Config: FilterExtraChannel = " + juce::String( node.attribute("value").as_int() ));
		}
	}
}

void XMLParser::cacheXmlMappings(pugi::xml_node mappingsNode)
{
	int channel;
	int key;
	int cacheKey;

	for (pugi::xml_node node = mappingsNode.child("mapping"); node; node = node.next_sibling("mapping")) {
		// logger.log("Documenting a mapping node");
		channel = node.attribute("channel").as_int();
		key = node.attribute("key").as_int();
		cacheKey = getCacheKey(channel, key);
		// logger.log("created cacheKey " + juce::String(channel) + "." + juce::String(key) + " = " + juce::String(cacheKey) + cacheKeyToChannelAndKeyString(cacheKey));

		Command instruction;
		instruction.command = juce::String( node.attribute("command").as_string() );

		for (pugi::xml_node param = node.child("param"); param; param = param.next_sibling("param")) {
			auto type = juce::String( param.attribute("type").as_string() );
			auto value = juce::String( param.attribute("value").as_string() );
			auto multiplierNode = param.attribute("multiplier");
			auto multiplier = multiplierNode ? multiplierNode.as_float() : (float)1.0;

			instruction.params.add(
				OscParam { type, value, multiplier }
			);
		}
		lookupMap.set(cacheKey, instruction);
	}

	logger.log("Created Mappings cache containing " + juce::String(lookupMap.size()) + " entries.");
}

const Command XMLParser::findCachedMapping(int channel, int note)
{
	auto cacheKey = getCacheKey(channel, note);
	if(lookupMap.contains(cacheKey)) {
		return lookupMap[cacheKey];
	} else {
		return Command {""};
	}
}

int XMLParser::countNodeChildren(pugi::xml_node& node, const char * name)
{
	if (juce::String(name).length() > 0)
	{
		auto& childIterator = node.children(name);
		return std::distance(childIterator.begin(), childIterator.end());
	}
	else
	{
		auto& childIterator = node.children();
		return std::distance(childIterator.begin(), childIterator.end());
	}
}

juce::String XMLParser::generateXmlDocumentation()
{
	//logger.log("Debug: Generate documentation");

	juce::String doc = "";
	juce::String channel;
	juce::String key;
	juce::String command;

	// lookupMap is unsorted, build an ordered list of contained keys
	LookupMapKeyComparator comparator;
	juce::Array<int> sortedCacheKeys;
	for (juce::HashMap<int, Command>::Iterator entry(lookupMap); entry.next();) {
		sortedCacheKeys.add(entry.getKey());
	}
	sortedCacheKeys.sort(comparator);

	for (const auto cacheKey : sortedCacheKeys) {
		auto& channelAndKey = cacheKeyToChannelAndKeyString(cacheKey);
		auto& instr = lookupMap[cacheKey];
		doc += "<Mapping> " + channelAndKey + " --> " + oscInstructionToString(instr) + "\n";
	}

	DBG("Successfully parsed file.");
	return doc;
}
