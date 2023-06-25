/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "../Components/MainComponent.h"

//==============================================================================
/**
*/
class JMidiTriggerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    JMidiTriggerAudioProcessorEditor (JMidiTriggerAudioProcessor&);
    ~JMidiTriggerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    // works because getAudioProcessor is defined in base class
    JMidiTriggerAudioProcessor* getProcessor() const { return static_cast <JMidiTriggerAudioProcessor*>(getAudioProcessor()); }

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    JMidiTriggerAudioProcessor& audioProcessor;
    std::unique_ptr<MainComponent> mainComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JMidiTriggerAudioProcessorEditor)
};
