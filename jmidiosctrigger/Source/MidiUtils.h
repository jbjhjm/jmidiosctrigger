/*
  ==============================================================================

    MidiUtils.h
    Created: 28 Jun 2023 12:34:36am
    Author:  Jannik

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>




namespace MidiUtils {

	struct MidiMessageAttributes {
		juce::String type;
		juce::String channel;
		juce::String key;
		juce::String value;
	};

	struct MidiMessageInfo {
		juce::String type;
		int channel;
		int key;
		int value;
	};


}

