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

#include "PluginEditor.h"
#include "EventLogComponent.h"
#include "AvailCmdsComponent.h"
#include "XmlGuideComponent.h"
#include "AboutComponent.h"
#include "resources.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
JMidiTriggerAudioProcessorEditor::JMidiTriggerAudioProcessorEditor (JMidiTriggerAudioProcessor& p)
    : AudioProcessorEditor(&p) , processor(p)
{ 
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

	addAndMakeVisible(selectFileButton = new TextButton("selectFileButton"));
	selectFileButton->setButtonText(TRANS("select MIDI data file"));
	selectFileButton->setConnectedEdges(Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
	selectFileButton->addListener(this);
	selectFileButton->setColour(TextButton::buttonColourId, Colour(0xff819f3c));
	selectFileButton->setColour(TextButton::buttonOnColourId, Colour(0xff8cbc30));
	selectFileButton->setColour(TextButton::textColourOnId, Colours::white);
	selectFileButton->setColour(TextButton::textColourOffId, Colours::white);

	addAndMakeVisible(refreshFileButton = new TextButton("refreshFileButton"));
	refreshFileButton->setButtonText(TRANS("reload"));
	refreshFileButton->setConnectedEdges(Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
	refreshFileButton->addListener(this);
	refreshFileButton->setColour(TextButton::buttonColourId, Colour(0xff819f3c));
	refreshFileButton->setColour(TextButton::buttonOnColourId, Colour(0xff8cbc30));
	refreshFileButton->setColour(TextButton::textColourOnId, Colours::white);
	refreshFileButton->setColour(TextButton::textColourOffId, Colours::white);

	addAndMakeVisible(filepathLabel = new Label("filepathLabel", TRANS("select a MIDI instructions XML file to use")));
	filepathLabel->setFont(Font(15.00f, Font::plain));
	filepathLabel->setJustificationType(Justification::centredLeft);
	filepathLabel->setEditable(false, false, false);
	filepathLabel->setColour(Label::backgroundColourId, Colour(0x00000000));
	filepathLabel->setColour(Label::textColourId, Colours::white);
	filepathLabel->setColour(TextEditor::textColourId, Colours::black);
	filepathLabel->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    addAndMakeVisible (tabbedComponent = new TabbedComponent (TabbedButtonBar::TabsAtBottom));
    tabbedComponent->setTabBarDepth (30);

	Components.add(new EventLogComponent(p,*this));
	tabbedComponent->addTab(TRANS("Event Log"), Colour(0xbb051015), Components[0], true);
	Components.add(new AvailCmdsComponent(p, *this));
	tabbedComponent->addTab(TRANS("Available Commands"), Colour(0xbb051015), Components[1], true);
	Components.add(new XmlGuideComponent(p, *this));
	tabbedComponent->addTab(TRANS("MIDI XML Example"), Colour(0xbb051015), Components[2], true);
	Components.add(new AboutComponent(p, *this));
	tabbedComponent->addTab(TRANS("About"), Colour(0xbb051015), Components[3], true);

    tabbedComponent->setCurrentTabIndex (0);

	cachedImage_background_png_1 = ImageCache::getFromMemory(resource_TestComponent_background_png, resource_TestComponent_background_png_size);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..

    //startTimer(200);//starts timer with interval of 200mS
	/*
	struct sFilepathListener : public Value::Listener
	{
		sFilepathListener::sFilepathListener(ScopedPointer<Label> p) :parent(p) {  }
		ScopedPointer<Label> parent;
		void valueChanged(Value & v)  { 
			// parent->setText(v.getValue().toString(), juce::NotificationType::sendNotification); 
		}
	};
	sFilepathListener filepathListener(filepathLabel);
	*/
	//p.xmlFilePath.addListener(this);
	filepathLabel->getTextValue().referTo(p.xmlFilePath);
	startTimer(200);
    //[/Constructor]
}

JMidiTriggerAudioProcessorEditor::~JMidiTriggerAudioProcessorEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    selectFileButton = nullptr;
	refreshFileButton = nullptr;
	filepathLabel = nullptr;
    tabbedComponent = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void JMidiTriggerAudioProcessorEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::black);

    //g.setColour (Colour (0x70b0d879));
    //g.setFont (Font ("Impact", 125.00f, Font::plain));
   // g.drawText (TRANS("SECTOR"),
   //             0, 55, getWidth() - 0, getHeight() - 75,
   //             Justification::centred, true);
	g.setOpacity(1.0f);
	g.drawImage(cachedImage_background_png_1,
		0, 0, getWidth(), getHeight(),
		0, 0, cachedImage_background_png_1.getWidth(), cachedImage_background_png_1.getHeight());

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void JMidiTriggerAudioProcessorEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

	selectFileButton->setBounds(getWidth() * 0.55, 10, getWidth()*0.25, 34);
	refreshFileButton->setBounds(getWidth() * 0.8 + 5, 10, getWidth()*0.2 - 10, 34);
	filepathLabel->setBounds(5, 46, getWidth() - 10, 34);
    tabbedComponent->setBounds (5, 85, getWidth() - 10, getHeight() - 100);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void JMidiTriggerAudioProcessorEditor::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

	if (buttonThatWasClicked == selectFileButton)
	{
		//[UserButtonCode_selectFileButton] -- add your button handler code here..
		showFileDialogue();
		//[/UserButtonCode_selectFileButton]
	}
	else if (buttonThatWasClicked == refreshFileButton)
	{
		//[UserButtonCode_selectFileButton] -- add your button handler code here..
		processor.reloadFile();
		//[/UserButtonCode_selectFileButton]
	}

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

/*void JMidiTriggerAudioProcessorEditor::valueChanged(Value& value)
{
	if (value == processor.xmlFilePath) {
		filepathLabel->setText(value.getValue().toString(), juce::NotificationType::sendNotification);
	}
}*/

//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void JMidiTriggerAudioProcessorEditor::timerCallback()
{
     JMidiTriggerAudioProcessor* p = getProcessor();
     //exchange any data you want between UI elements and the Plugin "ourProcessor"
	 for (int i = 0; i < Components.size(); i++) {
		 Components[i]->updateContents(*p,*this);
	 }

}

void JMidiTriggerAudioProcessorEditor::showFileDialogue()
{
  FileChooser dlg("Select MIDI Data file", File::getCurrentWorkingDirectory(), "*.xml");
  if (dlg.browseForFileToOpen()) {
    // selected file
    File filePath(dlg.getResult());
    processor.loadXmlFile(filePath);
  }
  else {
    // aborted
  }
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="JMidiTriggerAudioProcessorEditor"
                 componentName="" parentClasses="public AudioProcessorEditor, public Timer"
                 constructorParams="JMidiTriggerAudioProcessor&amp; OwnerFilter"
                 variableInitialisers="AudioProcessorEditor(ownerFilter)" snapPixels="8"
                 snapActive="1" snapShown="1" overlayOpacity="0.330" fixedSize="0"
                 initialWidth="400" initialHeight="300">
  <BACKGROUND backgroundColour="ff000000">
    <TEXT pos="0 55 0M 75M" fill="solid: 70b0d879" hasStroke="0" text="SECTOR"
          fontname="Impact" fontsize="125" bold="0" italic="0" justification="36"/>
  </BACKGROUND>
  <TEXTBUTTON name="selectFileButton" id="6675a29f18e4b172" memberName="selectFileButton"
              virtualName="" explicitFocusOrder="0" pos="195R 5 190 45" bgColOff="ff819f3c"
              bgColOn="ff8cbc30" textCol="ffffffff" textColOn="ffffffff" buttonText="select MIDI file"
              connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <LABEL name="filepathLabel" id="cca299c5275f99e5" memberName="filepathLabel"
         virtualName="" explicitFocusOrder="0" pos="5 5 200M 45" bkgCol="ff0c333c"
         textCol="ffffffff" edTextCol="ff000000" edBkgCol="0" labelText="select a MIDI instructions XML file to use"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  <TABBEDCOMPONENT name="new tabbed component" id="3723522eb742e890" memberName="tabbedComponent"
                   virtualName="" explicitFocusOrder="0" pos="5 55 10M 70M" orientation="bottom"
                   tabBarDepth="30" initialTab="0">
    <TAB name="Event Log" colour="5f197980" useJucerComp="1" contentClassName=""
         constructorParams="" jucerComponentFile="EventLogComponent.cpp"/>
    <TAB name="Available Commands" colour="5f197980" useJucerComp="1"
         contentClassName="" constructorParams="" jucerComponentFile="AvailCmdsComponent.cpp"/>
    <TAB name="MIDI XML Example" colour="5f197980" useJucerComp="1" contentClassName=""
         constructorParams="" jucerComponentFile="XmlGuideComponent.cpp"/>
  </TABBEDCOMPONENT>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
