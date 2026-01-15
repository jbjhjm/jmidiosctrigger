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
  */

#include "XMLParser.h"

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


bool XMLParser::loadXmlData(pugi::xml_document* doc)
{
	xmlReadyState = false;

	xmlDoc = doc;
	xmlRootNode = xmlDoc->document_element();
	if (!xmlRootNode) { DBG("Error: No XML root node found. "); return false; }
	DBG("Debug: Selected root node " + juce::String(xmlRootNode.name()));

	xmlConfigNode = xmlRootNode.child("config");

	xmlMappingsNode = xmlRootNode.child("mappings");
	if (!xmlMappingsNode) { DBG("Error: No XML <mappings> node found. "); return false; }
	DBG("Debug: Selected mappings group node " + juce::String(xmlMappingsNode.name()));

	xmlReadyState = true;
	return true;
}


pugi::xml_node XMLParser::findMappingNode(int channel, int note)
{
	// TODO: Pre-cache data for real time lookups
	// std::unordered_set or std::unordered_map are typically the fastest for average-case O(1) lookup when you need to check existence or retrieve values by key.
	// They use hashing, making them ideal for large datasets with frequent lookups, provided a good hash function is available.
	pugi::xpath_variable_set vars;
	vars.add("channel", pugi::xpath_value_type::xpath_type_number);
	vars.add("note", pugi::xpath_value_type::xpath_type_number);
	vars.set("channel", double(channel));
	vars.set("note", double(note));
	// TODO: instead of querying, a manual loop over children is likely more efficient
	pugi::xpath_query midiMappingQuery("mapping[@channel=number($channel)][@key=number($note)]", &vars);
	pugi::xpath_node targetNode = midiMappingQuery.evaluate_node(xmlMappingsNode);
	return targetNode.node();
}



// MidiUtils::MidiMessageInfo XMLParser::midiNodeToMidiMessageInfo(pugi::xml_node & midiNode, MidiUtils::MidiMessageInfo & inputInfo)
// {
// 	auto& midiNodeAttr = getMidiMessageAttributes(midiNode);

// 	MidiUtils::MidiMessageInfo info = {
// 		midiNodeAttr.type,
// 		1,
// 		0,
// 		0
// 	};

// 	std::map<juce::String, int> data;
// 	const pugi::char_t* keys[]{ "channel","key","value" };

// 	for (auto& key : keys) {
// 		juce::String strVal = MidiUtils::getPropFromMidiNodeAttributes(key, midiNodeAttr);
// 		bool isVar_dollar = strVal.startsWith("$");
// 		bool isVar_at = strVal.startsWith("@");
// 		int value;
// 		if (isVar_dollar || isVar_at) {
// 			auto varName = strVal.substring(1);
// 			if (isVar_dollar) {
// 				auto varNode = xmlVarsNode.child(varName.getCharPointer());
// 				value = varNode.attribute("value").as_int();
// 			}
// 			else 
// 			{
// 				value = MidiUtils::getPropFromMidiMessageInfo(varName, inputInfo);
// 			}
// 		}
// 		else 
// 		{
// 			value = strVal.getIntValue();
// 		}
// 		MidiUtils::setPropInMidiMessageInfo(key, value, info);
// 	}

// 	return info;
// }



bool XMLParser::findEntryforMidiEvent(juce::MidiMessage& inputInfo, pugi::xml_node& xmlEntry)
{
	xmlEntry = findMappingNode(inputInfo.getChannel(), inputInfo.getNoteNumber());
	const bool found = xmlEntry && xmlEntry.name() != "";
	return found;
}

int XMLParser::countNodeChildren(pugi::xml_node& node, const char * name = "")
{
	if (name != "")
	{
		auto& childIterator = node.children(name);
		return std::distance(childIterator.begin(), childIterator.end());
	}
	else
	{
		auto& childIterator = node.children(name);
		return std::distance(childIterator.begin(), childIterator.end());
	}
}

juce::String XMLParser::generateXmlDocumentation()
{
	if (!xmlRootNode) return "no configuration loaded, aborting doc generation";
	//logger.log("Debug: Generate documentation");

	juce::String doc = "";
	pugi::xml_node eventNode;
	juce::Array<pugi::string_t> eventIds;
	pugi::string_t eventName;

	//DBG("Debug: Selected events group node " );

	for (pugi::xml_node node = xmlMappingsNode.child("mapping"); node; node = node.next_sibling("mapping")) {
		// logger.log("Documenting a mapping node");

		// MidiUtils::MidiMessageAttributes info = getMidiMessageAttributes(node);

		// doc +=
		// 	"Mapping for Channel "
		// 	+ juce::String(info.channel) +
		// 	" [type=" + info.type +
		// 	"] [ " + juce::String(info.key) + " " + juce::String(info.value) + " ] " +
		// 	" ";

	}

	//midiDataInfo = doc;
	DBG("Successfully parsed file.");
	return doc;
}
