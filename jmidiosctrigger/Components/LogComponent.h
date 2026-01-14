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
#include "../Source/Store.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class LogComponent  : public juce::Component,
                      public juce::ValueTree::Listener,
                      private juce::AsyncUpdater,
                      public juce::Button::Listener
{
public:
    //==============================================================================
    LogComponent ();
    ~LogComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    juce::ValueTree logState;           // This is a valid node, of type "MyNode"
    void handleAsyncUpdate() override;
    void valueTreePropertyChanged(juce::ValueTree& v, const juce::Identifier& id) override;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::TextEditor> juce__textEditor;
    std::unique_ptr<juce::TextButton> clearButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LogComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

