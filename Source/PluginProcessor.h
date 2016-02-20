/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "pugixml.hpp" 
//#pragma comment( lib, "pugixml" )

/*struct MyListener : public Value::Listener
{
	Class& parent;
	void valueChanged(Value& v)  { parent.handleMyValueChanged(v); }
};*/


//==============================================================================
/**
*/
class JMidiTriggerAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    JMidiTriggerAudioProcessor();
    ~JMidiTriggerAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool silenceInProducesSilenceOut() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
	void setStateValue(const String key, const String value);
	String getStateValue(const String key, const String defaultValue);

    String currentStatus = "___";

	XmlElement pluginState;

    bool loadXmlFile(const String& filePath);
    bool loadXmlFile(const File& fi);
	void abortLoadXmlFile();
	bool reloadFile();
	void generateXmlDocumentation();
	bool loadXmlData();
	Array<pugi::string_t> getEventIdsForListener(const pugi::xml_node* listenerNode);

	void log(const String& txt);
	void logMidiMessage(const String& txt);
	void addMidiMessageToList(const MidiMessage& message, const String& source="");
	bool processMidiInputMessage(const MidiMessage& message, MidiBuffer& midiOutput);

	Value xmlFilePath;
	String statusLog;
	Value midiDataInfo;

	pugi::xml_document xmlDoc;
	pugi::xml_node xmlRootNode;
	pugi::xml_node xmlEventsNode;
	pugi::xml_node xmlListenersNode;
	bool xmlReadyState = false;

private:
    //==============================================================================



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JMidiTriggerAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
