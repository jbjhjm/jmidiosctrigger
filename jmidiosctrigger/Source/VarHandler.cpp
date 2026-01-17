/*
  ==============================================================================

    VarHandler.cpp
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

#include "VarHandler.h"




VarHandler& VarHandler::getInstance()
{
	static VarHandler instance;
	return instance;
}

//==============================================================================
VarHandler::VarHandler():
	logger (StatusLog::getInstance())
{
	DBG(">>>>>>>>>>>>>VarHandler::VarHandler");
}

VarHandler::~VarHandler()
{
}

void VarHandler::readXmlVariables(pugi::xml_node configNode)
{
	auto& configState = Store::getState(STATES::Config);
	auto x = FileUtils::countNodeChildren(configNode, "");
	logger.log("readXmlVariables: <variables> contains " + juce::String(x) + " children");

	juce::String name;
	juce::String type;

	for (pugi::xml_node node = configNode.first_child(); node; node = node.next_sibling()) {
		name = juce::String( node.attribute("name").as_string() );
		type = juce::String(node.attribute("type").as_string());
		juce::var value;
		if(type == "f") {
			value = node.attribute("default").as_double();
		} else if(type == "s") {
			value = juce::String(node.attribute("default").as_string());
		}
		// logger.log("Default var: "+name+"="+juce::String((double)value));
		variableDefaults.set(name,value);
	}
}
void VarHandler::resetDefaultVariables()
{
	variableDefaults.clear();
}
void VarHandler::resetVariables()
{
	variableAssignments.clear();
	logger.log("Reset dynamic vars");
}

const juce::var VarHandler::getVariable(juce::String name)
{
	if(variableAssignments.contains(name)) {
		// logger.log("Found var assignment for "+name);
		return variableAssignments[name];
	} else if(variableDefaults.contains(name)) {
		// logger.log("Found var default for "+name+": "+juce::String((float)variableDefaults[name]));
		return variableDefaults[name];
	} else {
		// logger.log("Found no var for "+name);
		juce::var emptyVal;
		return emptyVal;
	}
}

void VarHandler::setVariable(juce::String name, float value)
{
	juce::var variable = value;
	if(!variableDefaults.contains(name)) {
		logger.log("ERROR: tried to assign to unknown variable: "+name);
		return;
	}
	if(!variableDefaults[name].isDouble()) {
		logger.log("ERROR: tried to assign number to non-numeric variable: "+name);
		return;
	}
	variableAssignments.set(name, variable);
	logger.log("Assigned variable $" + name + "=" + juce::String(value));
}

bool VarHandler::command(const Command& instruction, juce::MidiMessage& midiInput)
{
	if(instruction.command == "~Reset") {
		resetVariables();
	} else if(instruction.command.startsWith("~SetValue")) {
		auto valueName = instruction.command.substring(10);
		setVariable(valueName, midiInput.getFloatVelocity() * 100);
	}
	return false;
}


