/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 7.0.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>
#include "../Source/PluginProcessor.h"
#include "../Source/FileUtils.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MainComponent  : public juce::Component,
                       public juce::Button::Listener,
                       public juce::ValueTree::Listener
{
public:
    //==============================================================================
    MainComponent (JMidiOscTriggerAudioProcessor& p);
    ~MainComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void timerCallback();
    void showFileDialogue();
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) override;
    void updateFilePathField();

    // Binary resources:
    static const char* background_png;
    static const int background_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    JMidiOscTriggerAudioProcessor* audioProcessor;
    std::unique_ptr<juce::FileChooser> chooser;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::TextButton> selectFileButton;
    std::unique_ptr<juce::TextButton> refreshFileButton;
    std::unique_ptr<juce::Label> filepathLabel;
    std::unique_ptr<juce::TabbedComponent> juce__tabbedComponent;
    juce::Image cachedImage_background_png_1;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

