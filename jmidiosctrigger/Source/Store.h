#pragma once
/*
  ==============================================================================

	StatusLog.cpp
	Created: 25 Jun 2023 7:16:07pm
	Author:  Jannik

  ==============================================================================

  Use getState with one of the Identifiers found in IDS.
  It will return the state, creating it on the fly if needed.
  As this is a constant class, no constructor passing is required.

  LOG state is temporary.
  CONFIG state will be persisted using hooks in PluginProcessor.
*/

#pragma once

#include "IDs.cpp"
#include <JuceHeader.h>
#include "StatusLog.h"

using namespace juce;


class Store
{
public:
	static ValueTree& getRoot();
	static ValueTree getState(Identifier id);
	static ValueTree importToState(Identifier id, juce::XmlElement* xmlData);

private:
	static ValueTree addState(Identifier id);

};

