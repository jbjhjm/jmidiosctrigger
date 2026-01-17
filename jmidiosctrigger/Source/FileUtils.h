/*
  ==============================================================================

    FileUtils.h
    Created: 22 Jun 2024
    Author:  Jannik

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../libraries/pugixml.hpp" 

namespace FileUtils {

	juce::String getRelativeFilePath(const juce::File& fi);

    juce::String solveRelativeFilePath(const juce::String& path);

	int countNodeChildren(pugi::xml_node& node, const char * name);
}
