/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 4.1.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "XmlGuideComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
XmlGuideComponent::XmlGuideComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (XmlGuideTextarea = new TextEditor ("XmlGuideTextarea"));
    XmlGuideTextarea->setMultiLine (true);
    XmlGuideTextarea->setReturnKeyStartsNewLine (true);
    XmlGuideTextarea->setReadOnly (true);
    XmlGuideTextarea->setScrollbarsShown (true);
    XmlGuideTextarea->setCaretVisible (false);
    XmlGuideTextarea->setPopupMenuEnabled (true);
    XmlGuideTextarea->setColour (TextEditor::textColourId, Colours::white);
    XmlGuideTextarea->setColour (TextEditor::backgroundColourId, Colour (0x000d2639));
    XmlGuideTextarea->setColour (TextEditor::shadowColourId, Colour (0x00000000));
    XmlGuideTextarea->setText (TRANS("bla bla"));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

XmlGuideComponent::~XmlGuideComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    XmlGuideTextarea = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void XmlGuideComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void XmlGuideComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    XmlGuideTextarea->setBounds (0, 0, getWidth() - 0, getHeight() - 0);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="XmlGuideComponent" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ffffff"/>
  <TEXTEDITOR name="XmlGuideTextarea" id="9164189f3c8afac2" memberName="XmlGuideTextarea"
              virtualName="" explicitFocusOrder="0" pos="0 0 0M 0M" textcol="ffffffff"
              bkgcol="d2639" shadowcol="0" initialText="bla bla" multiline="1"
              retKeyStartsLine="1" readonly="1" scrollbars="1" caret="0" popupmenu="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
