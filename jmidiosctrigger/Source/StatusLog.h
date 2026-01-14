/*
  ==============================================================================

    StatusLog.h
    Created: 28 Jun 2023 7:21:23pm
    Author:  Jannik

  ==============================================================================
*/

#pragma once

#include "IDs.cpp"
#include "Store.h"
#include <JuceHeader.h>

class StatusLog
{
	static const juce::Identifier logTreeIdentifier; // pre-create an Identifier

private:
	juce::String logData;

public:

	juce::ValueTree logTree;

	static StatusLog& getInstance();
	static juce::String getMidiMessageDescription(const juce::MidiMessage& m);

	StatusLog();

	~StatusLog();

	juce::String getLog();

	void addToLog(juce::String text, int indent = 0);

	void log(const juce::String& txt, int indent = 0);

	void debug(const juce::String& txt, int indent = 0);

	void logMidiMessage(const juce::MidiMessage& message, const juce::String& source);

};

juce::String makeIndentation(int indent);
