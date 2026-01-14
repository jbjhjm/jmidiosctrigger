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

//[Headers] You can add your own extra header files here...
#include "../Source/IDS.cpp"
//[/Headers]

#include "LogComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
LogComponent::LogComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    logState = Store::getState(STATES::Log);
    logState.addListener(this);


    //[/Constructor_pre]

    juce__textEditor.reset (new juce::TextEditor ("logField"));
    addAndMakeVisible (juce__textEditor.get());
    juce__textEditor->setMultiLine (true);
    juce__textEditor->setReturnKeyStartsNewLine (true);
    juce__textEditor->setReadOnly (true);
    juce__textEditor->setScrollbarsShown (true);
    juce__textEditor->setCaretVisible (false);
    juce__textEditor->setPopupMenuEnabled (true);
    juce__textEditor->setText (juce::String());

    clearButton.reset (new juce::TextButton ("clearButton"));
    addAndMakeVisible (clearButton.get());
    clearButton->setButtonText (TRANS("X clear"));
    clearButton->addListener (this);
    clearButton->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff2a2a2a));


    //[UserPreSize]
    juce__textEditor->applyFontToAllText(juce::Font{"Consolas", 14.f, 0});
    juce__textEditor->setLineSpacing(1.4f);
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
     // update initial value. valueTreePropertyChanged will trigger updates for future changes.
    triggerAsyncUpdate();
    //[/Constructor]
}

LogComponent::~LogComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    juce__textEditor = nullptr;
    clearButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void LogComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void LogComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    juce__textEditor->setBounds (0, 0, proportionOfWidth (1.0000f), proportionOfHeight (1.0000f));
    clearButton->setBounds (getWidth() - 85, 15, 70, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void LogComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == clearButton.get())
    {
        //[UserButtonCode_clearButton] -- add your button handler code here..
        auto& logState = Store::getState(STATES::Log);
        logState.setProperty(PROPS::LogData, "", nullptr);
        //[/UserButtonCode_clearButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

/*void LogComponent::updateContents(JMidiOscTriggerAudioProcessor& p, JMidiOscTriggerAudioProcessorEditor& e)
{
    if (p.statusLog != "") {
        String newContent = p.statusLog;
        EventlogTextarea->moveCaretToEnd();
        EventlogTextarea->insertTextAtCaret(newContent);
        EventlogTextarea->scrollDown();
    }
    p.statusLog = "";
}*/

void LogComponent::handleAsyncUpdate()
{
    DBG("HandleAsyncUpdate");
    juce::String newContent = logState.getProperty(PROPS::LogData);
    juce__textEditor->setText(newContent);
    juce__textEditor->moveCaretToEnd();
    //juce__textEditor->insertTextAtCaret(newContent);
  //  juce__textEditor->scrollDown();
    juce__textEditor->setScrollToShowCursor(true);
   // juce__textEditor->scrollEditorToPositionCaret(0, 0);
}

void LogComponent::valueTreePropertyChanged(juce::ValueTree& v, const juce::Identifier& id)
{
    // DBG("valueTreePropertyChanged: " + v.getType().toString() + " " + id.toString());
    // v.hasType(IDs::LogTree)
    //if (id == PROPS::LogData) {
        triggerAsyncUpdate(); // calls handleAsyncUpdate
    //}
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="LogComponent" componentName=""
                 parentClasses="public juce::Component, public juce::ValueTree::Listener, private juce::AsyncUpdater"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="600"
                 initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TEXTEDITOR name="logField" id="3f5e945e4813c139" memberName="juce__textEditor"
              virtualName="" explicitFocusOrder="0" pos="0 0 100% 100%" initialText=""
              multiline="1" retKeyStartsLine="1" readonly="1" scrollbars="1"
              caret="0" popupmenu="1"/>
  <TEXTBUTTON name="clearButton" id="7602d80f8b4fa372" memberName="clearButton"
              virtualName="" explicitFocusOrder="0" pos="85R 15 70 24" bgColOff="ff2a2a2a"
              buttonText="X clear" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

