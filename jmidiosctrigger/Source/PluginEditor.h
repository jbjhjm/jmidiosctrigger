/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "../Components/MainComponent.h"

//==============================================================================
/**
*/
class JMidiOscTriggerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    JMidiOscTriggerAudioProcessorEditor (JMidiOscTriggerAudioProcessor&);
    ~JMidiOscTriggerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    // works because getAudioProcessor is defined in base class
    JMidiOscTriggerAudioProcessor* getProcessor() const { return static_cast <JMidiOscTriggerAudioProcessor*>(getAudioProcessor()); }

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    JMidiOscTriggerAudioProcessor& audioProcessor;
    std::unique_ptr<MainComponent> mainComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JMidiOscTriggerAudioProcessorEditor)
};
