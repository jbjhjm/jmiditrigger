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

#ifndef __JUCE_HEADER_507BFAA26CC5FEC2__
#define __JUCE_HEADER_507BFAA26CC5FEC2__

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
class EventLogComponent : public UpdateableComponent, public ValueListener
{
public:
    //==============================================================================
	EventLogComponent(JMidiTriggerAudioProcessor& p, JMidiTriggerAudioProcessorEditor& e);
    ~EventLogComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void valueChanged(Value &value) override;
	//[/UserMethods]

    void paint (Graphics& g);
    void resized();

	Value statusLog;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<TextEditor> EventlogTextarea;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EventLogComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_507BFAA26CC5FEC2__
