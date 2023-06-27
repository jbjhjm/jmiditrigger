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

#include "DocComponent.h"
#include "../Source/XMLReader.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
DocComponent::DocComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    juce__textEditor.reset (new juce::TextEditor ("new text editor"));
    addAndMakeVisible (juce__textEditor.get());
    juce__textEditor->setMultiLine (false);
    juce__textEditor->setReturnKeyStartsNewLine (false);
    juce__textEditor->setReadOnly (false);
    juce__textEditor->setScrollbarsShown (true);
    juce__textEditor->setCaretVisible (true);
    juce__textEditor->setPopupMenuEnabled (true);
    juce__textEditor->setText (juce::String());

    juce__textEditor->setBounds (369, 320, 150, 24);

    docField.reset (new juce::TextEditor ("docField"));
    addAndMakeVisible (docField.get());
    docField->setMultiLine (true);
    docField->setReturnKeyStartsNewLine (true);
    docField->setReadOnly (true);
    docField->setScrollbarsShown (true);
    docField->setCaretVisible (false);
    docField->setPopupMenuEnabled (true);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..

    XMLReader& xmlReader = XMLReader::getInstance();
    docField->getTextValue().referTo(xmlReader.documentation);

    //[/Constructor]
}

DocComponent::~DocComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    juce__textEditor = nullptr;
    docField = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void DocComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void DocComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    docField->setBounds (0, 0, proportionOfWidth (1.0000f), proportionOfHeight (1.0000f));
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

<JUCER_COMPONENT documentType="Component" className="DocComponent" componentName=""
                 parentClasses="public juce::Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TEXTEDITOR name="new text editor" id="1bb0c266b0d7100b" memberName="juce__textEditor"
              virtualName="" explicitFocusOrder="0" pos="369 320 150 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="docField" id="3f5e945e4813c139" memberName="docField" virtualName=""
              explicitFocusOrder="0" pos="0 0 100% 100%" initialText="" multiline="1"
              retKeyStartsLine="1" readonly="1" scrollbars="1" caret="0" popupmenu="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

