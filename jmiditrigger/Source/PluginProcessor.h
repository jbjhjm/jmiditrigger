/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "XMLReader.h"
#include "../libraries/pugixml.hpp" 
#include "StatusLog.cpp"

//==============================================================================
/**
*/
class JMidiTriggerAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    JMidiTriggerAudioProcessor();
    ~JMidiTriggerAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //============================================================================== CUSTOM START

    void setStateValue(juce::Identifier& key, const juce::String& value);
    juce::String getStateValue(juce::Identifier& key, const juce::String& defaultValue);
    // String currentStatus = "___";

    bool loadXmlFile(const juce::String& filePath);
    bool loadXmlFile(const juce::File& fi);
    bool reloadFile();
    void generateXmlDocumentation();
    juce::Array<pugi::string_t> getEventIdsForListener(const pugi::xml_node* listenerNode);

    void addMidiMessageToList(const juce::MidiMessage& message, const juce::String& source = "");
    bool processMidiInputMessage(const juce::MidiMessage& message, juce::MidiBuffer& midiOutput);

    XMLReader xmlReader;
    StatusLog logger;

    juce::Value midiDataInfo;



private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JMidiTriggerAudioProcessor)
};
