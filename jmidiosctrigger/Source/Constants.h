#pragma once


#ifdef _DEBUG
const juce::String BUILD_TYPE = "Debug";
#else
const juce::String BUILD_TYPE = "Release";
#endif


const juce::String PLUGIN_VERSION = "1.1.0 " + BUILD_TYPE;
const juce::String BUILD_DATE = __DATE__ " " __TIME__;

const int MAX_LOG_LENGTH = 10000;