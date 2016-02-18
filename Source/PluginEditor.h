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

#ifndef __JUCE_HEADER_B633E1298AD05EAC__
#define __JUCE_HEADER_B633E1298AD05EAC__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "PluginProcessor.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class JMidiTriggerAudioProcessorEditor  : public AudioProcessorEditor,
	public Timer,
	//public ValueListener,
	public ButtonListener
{
public:
    //==============================================================================
    JMidiTriggerAudioProcessorEditor (JMidiTriggerAudioProcessor& p);
    ~JMidiTriggerAudioProcessorEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void timerCallback();
    JMidiTriggerAudioProcessor* getProcessor() const {return static_cast <JMidiTriggerAudioProcessor*>(getAudioProcessor());}
    void showFileDialogue();
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
	//void valueChanged(Value &value) override;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
	JMidiTriggerAudioProcessor& processor;
	ScopedPointer<TextButton> selectFileButton;
	ScopedPointer<TextButton> refreshFileButton;
    ScopedPointer<Label> filepathLabel;
    ScopedPointer<TabbedComponent> tabbedComponent;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JMidiTriggerAudioProcessorEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_B633E1298AD05EAC__
