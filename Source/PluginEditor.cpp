/*
  ==============================================================================

  ==============================================================================
*/

//#include <string>
//#include <cstdio>

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ChooseFileButton.h"
//#include "rapidjson/document.h"

//#include "rapidjson/filereadstream.h"


//==============================================================================
JMidiTriggerAudioProcessorEditor::JMidiTriggerAudioProcessorEditor (JMidiTriggerAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{

	//addAndMakeVisible(selectFileButton);
	//selectFileButton.setButtonText("Choose MIDI Events file");
	//selectFileButton.addListener(this);
	addAndMakeVisible(ChooseFileButton());

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (500, 300);

	//this->currentStatus = "";

	// String filePath = File::getCurrentWorkingDirectory().getFullPathName() + "\\test.xml";
	// this->processor.loadXmlFile(filePath);

}

JMidiTriggerAudioProcessorEditor::~JMidiTriggerAudioProcessorEditor()
{
}

//==============================================================================
void JMidiTriggerAudioProcessorEditor::paint (Graphics& g)
{

	g.fillAll(Colours::white);
	g.setColour(Colours::black);
    g.setFont (15.0f);
	g.drawFittedText(processor.currentStatus, getLocalBounds(), Justification::centred, 1);

}

void JMidiTriggerAudioProcessorEditor::resized()
{
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..
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
