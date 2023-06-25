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

#ifndef __JUCE_HEADER_E0D764B226C9895614__
#define __JUCE_HEADER_E0D764B226C9895614__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"

//[/Headers]

class UpdateableComponent : public Component
{
public:

	UpdateableComponent(JMidiTriggerAudioProcessor& p, JMidiTriggerAudioProcessorEditor& e);
	~UpdateableComponent();

	virtual void updateContents(JMidiTriggerAudioProcessor& p, JMidiTriggerAudioProcessorEditor& e);

private:

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UpdateableComponent)

};

#endif   // __JUCE_HEADER_E0D764B226C9895614__
