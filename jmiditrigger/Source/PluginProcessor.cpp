/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JMidiTriggerAudioProcessor::JMidiTriggerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
     ),
	logger (StatusLog::getInstance())
#endif
{
	logger.log("Application started");
}

JMidiTriggerAudioProcessor::~JMidiTriggerAudioProcessor()
{
}

//==============================================================================
const juce::String JMidiTriggerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JMidiTriggerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool JMidiTriggerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool JMidiTriggerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double JMidiTriggerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JMidiTriggerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int JMidiTriggerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void JMidiTriggerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String JMidiTriggerAudioProcessor::getProgramName (int index)
{
    return {};
}

void JMidiTriggerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void JMidiTriggerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void JMidiTriggerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool JMidiTriggerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    juce::ignoreUnused (layouts);
    return true;
}
#endif

void JMidiTriggerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    juce::MidiBuffer midiOutput;
    int time;
    juce::MidiMessage m;
    juce::String searchString;

    for (juce::MidiBuffer::Iterator i(midiMessages); i.getNextEvent(m, time);)
    {
		logger.logMidiMessage(m, "processBlock");
        processMidiInputMessage(m, midiOutput);
    }

    midiMessages.swapWith(midiOutput);
}

//==============================================================================
bool JMidiTriggerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* JMidiTriggerAudioProcessor::createEditor()
{
    return new JMidiTriggerAudioProcessorEditor (*this);
}

//==============================================================================
void JMidiTriggerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto& configState = Store::getState(STATES::Config);
    //DBG(configState.toXmlString());
    //DBG(configState.getProperty(CONFIGPROPS::FilePath).toString());
    std::unique_ptr<juce::XmlElement> configStateAsXml (configState.createXml());
    //DBG(configStateAsXml->toString());
    copyXmlToBinary(*configStateAsXml, destData);
}

void JMidiTriggerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // getXmlFromBinary returns a unique_ptr. Need to convert it to a regular pointer to allow passing data to importToState.
    auto configStateAsXml = getXmlFromBinary(data, sizeInBytes);
    if (configStateAsXml.get() == nullptr) return;
    juce::XmlElement* regularXmlPointer = configStateAsXml.release();

    auto configState = Store::importToState(STATES::Config, regularXmlPointer);

    delete regularXmlPointer;

	// TODO: instead of triggering manual file updates from multiple points,
	// a State listener should be added to react accordingly and trigger XML reload.

	auto filePath = configState.getProperty(CONFIGPROPS::FilePath);
	if(filePath.isString()) {
		loadXmlFile(filePath.toString());
	}

}


//==============================================================================


bool JMidiTriggerAudioProcessor::loadXmlFile(const juce::File& fi)
{
	return XMLReader::getInstance().loadXmlFile(fi);
}

bool JMidiTriggerAudioProcessor::loadXmlFile(const juce::String& filePath)
{
	return XMLReader::getInstance().loadXmlFile(filePath);
}

bool JMidiTriggerAudioProcessor::reloadFile()
{
	return XMLReader::getInstance().reloadFile();
}





auto JMidiTriggerAudioProcessor::getMidiMessageTypeAndKey(const juce::MidiMessage& message)
{
	juce::String type = "";
	int channel = 0;
	int key = 0;
    int value = 0;

	if (message.isNoteOn())
	{
		type = "noteon";
		key = message.getNoteNumber();
        value = message.getVelocity();
	}
	else if (message.isNoteOff())
	{
		type = "noteoff";
		key = message.getNoteNumber();
        value = message.getVelocity();
    }
	else if (message.isController())
	{
		type = "cc";
		key = message.getControllerNumber();
        value = message.getControllerValue();
    }
	else if (message.isProgramChange())
	{
		type = "pc";
		key = message.getProgramChangeNumber();
	}

    struct result { juce::String type; int key; int value; };
	return result{ type, key, value };

}




bool JMidiTriggerAudioProcessor::processMidiInputMessage(const juce::MidiMessage& message, juce::MidiBuffer& midiOutput)
{
	//<listener channel = "2" type = "cc" key = "11" >
	//const MidiMessage m = MidiMessage::noteOn(message.getChannel(), message.getNoteNumber(), newVel);

    XMLReader& xmlReader = XMLReader::getInstance();
    if (!xmlReader.isReady()) {
        logger.log("Error: No configuration is loaded!",1);
        return false;
    }

	auto [type, key, value] = getMidiMessageTypeAndKey(message);
	int channel = message.getChannel();

	bool foundAnyData = false;

    MidiUtils::MidiMessageInfo inputInfo {
        type,
        channel,
        key,
        value
    };

    return xmlReader.parser->handleMidiEvent(inputInfo, midiOutput);
}





//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JMidiTriggerAudioProcessor();
}
