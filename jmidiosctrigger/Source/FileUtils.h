/*
  ==============================================================================

    FileUtils.h
    Created: 22 Jun 2024
    Author:  Jannik

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace FileUtils {

	juce::String getRelativeFilePath(const juce::File& fi);

    juce::String solveRelativeFilePath(const juce::String& path);

}
