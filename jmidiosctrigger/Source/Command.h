/*
  ==============================================================================

    Command.h
    Created: 17 Jan 2026 10:53:28am
    Author:  JDESKTOP

  ==============================================================================
*/

#pragma once

struct OscParam {
	juce::String type;
	juce::String value;
	float multiplier;
};

struct Command {
	juce::String command;
	juce::Array<OscParam> params;
};