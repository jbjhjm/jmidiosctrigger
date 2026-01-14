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
//[/Headers]

#include "DemoComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
DemoComponent::DemoComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    DemoField.reset (new juce::TextEditor ("DemoField"));
    addAndMakeVisible (DemoField.get());
    DemoField->setMultiLine (true);
    DemoField->setReturnKeyStartsNewLine (true);
    DemoField->setReadOnly (true);
    DemoField->setScrollbarsShown (true);
    DemoField->setCaretVisible (false);
    DemoField->setPopupMenuEnabled (true);
    DemoField->setText (TRANS("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
    "<jmididata>\n"
    "\n"
    "\t<variables>\n"
    "\t\t<myvar value=\"11\"/>\n"
    "\t\t<axeChannel value=\"7\"/>\n"
    "\t</variables>\n"
    "  \n"
    "\t<events>\n"
    "\t\t<event id=\"ctrl1\">\t\t<midi type=\"cc\" channel=\"3\" key=\"016\" value=\"127\" /></event>\n"
    "\t\t<event id=\"ctrl2\">\t\t<midi type=\"cc\" channel=\"3\" key=\"017\" value=\"$myvar\" /></event>\n"
    "\t\t<event id=\"preset1\">\t<midi type=\"pc\" channel=\"$axeChannel\" key=\"050\" value=\"@value\" /></event>\n"
    "\t\t<event id=\"multievent\">\t\n"
    "\t\t\t<midi type=\"pc\" channel=\"3\" key=\"052\" value=\"127\" />\n"
    "\t\t\t<midi type=\"cc\" channel=\"3\" key=\"052\" value=\"111\" />\n"
    "\t\t\t<midi type=\"noteon\" channel=\"3\" key=\"052\" value=\"@value\" />\n"
    "\t\t\t<midi type=\"noteon\" channel=\"@channel\" key=\"052\" value=\"@value\" />\n"
    "\t\t</event>\n"
    "\t</events>\n"
    "\n"
    "\t<listeners>\n"
    "\t\t<listener channel=\"1\" type=\"all\" key=\"036\" trigger=\"ctrl1\" />\n"
    "\t\t<listener channel=\"1\" type=\"noteon\" key=\"037\" trigger=\"ctrl2\" />\n"
    "\t\t<listener channel=\"1\" type=\"noteon\" key=\"038\" trigger=\"preset1\" />\n"
    "\t\t<listener channel=\"1\" type=\"noteon\" key=\"039\" trigger=\"multievent\" />\n"
    "\t\t<listener channel=\"1\" type=\"noteon\" key=\"040\" ><midi type=\"pc\" channel=\"$axeChannel\" key=\"050\" value=\"127\" /></listener>\n"
    "\t\t<listener channel=\"1\" type=\"noteoff\" key=\"041\" ><midi type=\"pc\" channel=\"$axeChannel\" key=\"@key\" value=\"50\" /></listener>\n"
    "\t\t<listener channel=\"1\" type=\"noteoff\" key=\"042\" ><midi type=\"cc\" channel=\"3\" key=\"018\" value=\"@value\" /></listener>\n"
    "\t</listeners>\n"
    "  \n"
    "</jmididata>"));


    //[UserPreSize]
    DemoField->applyFontToAllText(juce::Font{"Consolas", 14.f, 0});
    DemoField->setLineSpacing(1.4f);
    DemoField->setText(TRANS("\n"
        "Known midi input types: noteon | noteoff | pc | cc\n"
        "@var accesses properties of source midi event. Supported: @value @channel @key \n"
        "$var is used to access custom variable declarations, see below. \n"
        "(see also sourcecode of MidiUtils\n"
        "\n"
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
        "<jmididata>\n"
        "\n"
        "\t<variables>\n"
        "\t\t<myvar value=\"11\"/>\n"
        "\t\t<axeChannel value=\"7\"/>\n"
        "\t</variables>\n"
        "  \n"
        "\t<events>\n"
        "\t\t<event id=\"ctrl1\">\t\t<midi type=\"cc\" channel=\"3\" key=\"016\" value=\"127\" /></event>\n"
        "\t\t<event id=\"ctrl2\">\t\t<midi type=\"cc\" channel=\"3\" key=\"017\" value=\"$myvar\" /></event>\n"
        "\t\t<event id=\"preset1\">\t<midi type=\"pc\" channel=\"$axeChannel\" key=\"050\" value=\"@value\" /></event>\n"
        "\t\t<event id=\"multievent\">\t\n"
        "\t\t\t<midi type=\"pc\" channel=\"3\" key=\"052\" value=\"127\" />\n"
        "\t\t\t<midi type=\"cc\" channel=\"3\" key=\"052\" value=\"111\" />\n"
        "\t\t\t<midi type=\"noteon\" channel=\"3\" key=\"052\" value=\"@value\" />\n"
        "\t\t\t<midi type=\"noteon\" channel=\"@channel\" key=\"052\" value=\"@value\" />\n"
        "\t\t</event>\n"
        "\t</events>\n"
        "\n"
        "\t<listeners>\n"
        "\t\t<listener channel=\"1\" type=\"all\" key=\"036\" trigger=\"ctrl1\" />\n"
        "\t\t<listener channel=\"1\" type=\"noteon\" key=\"037\" trigger=\"ctrl2\" />\n"
        "\t\t<listener channel=\"1\" type=\"noteon\" key=\"038\" trigger=\"preset1\" />\n"
        "\t\t<listener channel=\"1\" type=\"noteon\" key=\"039\" trigger=\"multievent\" />\n"
        "\t\t<listener channel=\"1\" type=\"noteon\" key=\"040\" ><midi type=\"pc\" channel=\"$axeChannel\" key=\"050\" value=\"127\" /></listener>\n"
        "\t\t<listener channel=\"1\" type=\"noteoff\" key=\"041\" ><midi type=\"pc\" channel=\"$axeChannel\" key=\"@key\" value=\"50\" /></listener>\n"
        "\t\t<listener channel=\"1\" type=\"noteoff\" key=\"042\" ><midi type=\"cc\" channel=\"3\" key=\"018\" value=\"@value\" /></listener>\n"
        "\t</listeners>\n"
        "  \n"
        "</jmididata>"));
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

DemoComponent::~DemoComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    DemoField = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void DemoComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void DemoComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    DemoField->setBounds (0, 0, proportionOfWidth (1.0000f), proportionOfHeight (1.0000f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="DemoComponent" componentName=""
                 parentClasses="public juce::Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TEXTEDITOR name="DemoField" id="3f5e945e4813c139" memberName="DemoField"
              virtualName="" explicitFocusOrder="0" pos="0 0 100% 100%" initialText="&lt;?xml version=&quot;1.0&quot; encoding=&quot;utf-8&quot;?&gt;&#10;&lt;jmididata&gt;&#10;&#10;&#9;&lt;variables&gt;&#10;&#9;&#9;&lt;myvar value=&quot;11&quot;/&gt;&#10;&#9;&#9;&lt;axeChannel value=&quot;7&quot;/&gt;&#10;&#9;&lt;/variables&gt;&#10;  &#10;&#9;&lt;events&gt;&#10;&#9;&#9;&lt;event id=&quot;ctrl1&quot;&gt;&#9;&#9;&lt;midi type=&quot;cc&quot; channel=&quot;3&quot; key=&quot;016&quot; value=&quot;127&quot; /&gt;&lt;/event&gt;&#10;&#9;&#9;&lt;event id=&quot;ctrl2&quot;&gt;&#9;&#9;&lt;midi type=&quot;cc&quot; channel=&quot;3&quot; key=&quot;017&quot; value=&quot;$myvar&quot; /&gt;&lt;/event&gt;&#10;&#9;&#9;&lt;event id=&quot;preset1&quot;&gt;&#9;&lt;midi type=&quot;pc&quot; channel=&quot;$axeChannel&quot; key=&quot;050&quot; value=&quot;@value&quot; /&gt;&lt;/event&gt;&#10;&#9;&#9;&lt;event id=&quot;multievent&quot;&gt;&#9;&#10;&#9;&#9;&#9;&lt;midi type=&quot;pc&quot; channel=&quot;3&quot; key=&quot;052&quot; value=&quot;127&quot; /&gt;&#10;&#9;&#9;&#9;&lt;midi type=&quot;cc&quot; channel=&quot;3&quot; key=&quot;052&quot; value=&quot;111&quot; /&gt;&#10;&#9;&#9;&#9;&lt;midi type=&quot;noteon&quot; channel=&quot;3&quot; key=&quot;052&quot; value=&quot;@value&quot; /&gt;&#10;&#9;&#9;&#9;&lt;midi type=&quot;noteon&quot; channel=&quot;@channel&quot; key=&quot;052&quot; value=&quot;@value&quot; /&gt;&#10;&#9;&#9;&lt;/event&gt;&#10;&#9;&lt;/events&gt;&#10;&#10;&#9;&lt;listeners&gt;&#10;&#9;&#9;&lt;listener channel=&quot;1&quot; type=&quot;all&quot; key=&quot;036&quot; trigger=&quot;ctrl1&quot; /&gt;&#10;&#9;&#9;&lt;listener channel=&quot;1&quot; type=&quot;noteon&quot; key=&quot;037&quot; trigger=&quot;ctrl2&quot; /&gt;&#10;&#9;&#9;&lt;listener channel=&quot;1&quot; type=&quot;noteon&quot; key=&quot;038&quot; trigger=&quot;preset1&quot; /&gt;&#10;&#9;&#9;&lt;listener channel=&quot;1&quot; type=&quot;noteon&quot; key=&quot;039&quot; trigger=&quot;multievent&quot; /&gt;&#10;&#9;&#9;&lt;listener channel=&quot;1&quot; type=&quot;noteon&quot; key=&quot;040&quot; &gt;&lt;midi type=&quot;pc&quot; channel=&quot;$axeChannel&quot; key=&quot;050&quot; value=&quot;127&quot; /&gt;&lt;/listener&gt;&#10;&#9;&#9;&lt;listener channel=&quot;1&quot; type=&quot;noteoff&quot; key=&quot;041&quot; &gt;&lt;midi type=&quot;pc&quot; channel=&quot;$axeChannel&quot; key=&quot;@key&quot; value=&quot;50&quot; /&gt;&lt;/listener&gt;&#10;&#9;&#9;&lt;listener channel=&quot;1&quot; type=&quot;noteoff&quot; key=&quot;042&quot; &gt;&lt;midi type=&quot;cc&quot; channel=&quot;3&quot; key=&quot;018&quot; value=&quot;@value&quot; /&gt;&lt;/listener&gt;&#10;&#9;&lt;/listeners&gt;&#10;  &#10;&lt;/jmididata&gt;"
              multiline="1" retKeyStartsLine="1" readonly="1" scrollbars="1"
              caret="0" popupmenu="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

