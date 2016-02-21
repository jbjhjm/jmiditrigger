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

#include "EventLogComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
EventLogComponent::EventLogComponent(JMidiTriggerAudioProcessor& p, JMidiTriggerAudioProcessorEditor& e)
	: UpdateableComponent(p, e)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (EventlogTextarea = new TextEditor ("EventlogTextarea"));
    EventlogTextarea->setMultiLine (true);
    EventlogTextarea->setReturnKeyStartsNewLine (true);
    EventlogTextarea->setReadOnly (true);
    EventlogTextarea->setScrollbarsShown (true);
    EventlogTextarea->setCaretVisible (false);
    EventlogTextarea->setPopupMenuEnabled (false);
    EventlogTextarea->setColour (TextEditor::textColourId, Colours::white);
	EventlogTextarea->setColour(TextEditor::backgroundColourId, Colour(0x00000000));
    EventlogTextarea->setColour (TextEditor::shadowColourId, Colour (0x00000000));
    EventlogTextarea->setText (TRANS(""));

	addAndMakeVisible(ClearLogButton = new TextButton("clearLogButton"));
	ClearLogButton->setButtonText(TRANS("clear log"));
	ClearLogButton->setConnectedEdges(Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
	ClearLogButton->addListener(this);
	ClearLogButton->setColour(TextButton::buttonColourId, Colour(0x30819f3c));
	ClearLogButton->setColour(TextButton::buttonOnColourId, Colour(0x308cbc30));
	ClearLogButton->setColour(TextButton::textColourOnId, Colours::white);
	ClearLogButton->setColour(TextButton::textColourOffId, Colours::white);

    //[UserPreSize]
	//statusLog.referTo(p.statusLog);
	//statusLog.addListener(this);
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

EventLogComponent::~EventLogComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    EventlogTextarea = nullptr;
	ClearLogButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void EventLogComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void EventLogComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    EventlogTextarea->setBounds (0, 0, getWidth() - 0, getHeight() - 0);
	ClearLogButton->setBounds(getWidth() -75, getHeight()-35, 70, 30);
	//[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void EventLogComponent::buttonClicked(Button* buttonThatWasClicked)
{
	if (buttonThatWasClicked == ClearLogButton)
	{
		EventlogTextarea->clear();
	}
}


//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
// @TODO replace by timer. immediate updates may lead to errors when too much / too fast inpuit is generated
void EventLogComponent::valueChanged(Value& value)
{
	/*String Content = EventlogTextarea->getText();
	Content.append( value.getValue().toString(), 2000 );
	EventlogTextarea->setText( Content, juce::NotificationType::sendNotification);
	EventlogTextarea->moveCaretToEnd();
	EventlogTextarea->scrollDown();
	value = "";*/
}

void EventLogComponent::updateContents(JMidiTriggerAudioProcessor& p, JMidiTriggerAudioProcessorEditor& e)
{
	if (p.statusLog != "") {
		String newContent = p.statusLog;
		EventlogTextarea->moveCaretToEnd();
		EventlogTextarea->insertTextAtCaret(newContent);
		EventlogTextarea->scrollDown();
	}
	p.statusLog = "";
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="EventLogComponent" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="42571c"/>
  <TEXTEDITOR name="EventlogTextarea" id="9164189f3c8afac2" memberName="EventlogTextarea"
              virtualName="" explicitFocusOrder="0" pos="0 0 0M 0M" textcol="ffffffff"
              bkgcol="c2131" shadowcol="0" initialText="bla bla" multiline="1"
              retKeyStartsLine="1" readonly="1" scrollbars="1" caret="0" popupmenu="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
