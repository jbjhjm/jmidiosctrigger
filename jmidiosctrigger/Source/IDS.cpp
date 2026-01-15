/*
  ==============================================================================

    IDS.cpp
    Created: 25 Jun 2023 9:34:39pm
    Author:  Jannik

  ==============================================================================
*/

#pragma once

//#include "StatusLog.h"
#include <JuceHeader.h>

namespace IDs
{
    const juce::Identifier ROOT_STORE("ROOT_STORE");
}

namespace STATES
{
    const juce::Identifier Log("Log");
    const juce::Identifier Config("Config");
}

namespace PROPS
{
    const juce::Identifier LogData("LogData");
}

namespace CONFIGPROPS
{
    const juce::Identifier FilePath("FilePath");
    const juce::Identifier IP("IP");
    const juce::Identifier Port("Port");
}