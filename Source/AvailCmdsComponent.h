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

#ifndef __JUCE_HEADER_E0D764B226C98956__
#define __JUCE_HEADER_E0D764B226C98956__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "UpdateableComponent.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class AvailCmdsComponent : public UpdateableComponent, public ValueListener
{
public:
    //==============================================================================
	AvailCmdsComponent(JMidiTriggerAudioProcessor& p, JMidiTriggerAudioProcessorEditor& e);
    ~AvailCmdsComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void valueChanged(Value &value) override;
	//[/UserMethods]

    void paint (Graphics& g);
    void resized();
	Value midiDataInfo;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<TextEditor> availCmdsTextarea;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AvailCmdsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_E0D764B226C98956__
