/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JMidiOscTriggerAudioProcessorEditor::JMidiOscTriggerAudioProcessorEditor (JMidiOscTriggerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 400);
   // MainComponent* x = new MainComponent(p);
    mainComponent = std::make_unique<MainComponent>(p);
   // this->mainComponent = new MainComponent(p);
    addAndMakeVisible(mainComponent.get());
}

JMidiOscTriggerAudioProcessorEditor::~JMidiOscTriggerAudioProcessorEditor()
{
}

//==============================================================================
void JMidiOscTriggerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void JMidiOscTriggerAudioProcessorEditor::resized()
{
    // fit main Component into Editor view
    // // crashes for some reason.
    //(*mainComponent.get()).setBounds(getLocalBounds());
}
