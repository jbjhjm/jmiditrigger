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
                       )
#endif
{
	logger = StatusLog::getInstance();
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
    //XmlElement pluginState("pluginState");

    // add some attributes to it..
    //pluginState.setAttribute("xmlFilePath", xmlFilePath.getValue().toString());
    //copyXmlToBinary(pluginState, destData);
}

void JMidiTriggerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    /*XmlElement* const pluginState = getXmlFromBinary(data, sizeInBytes);
    if (pluginState) {
        //delete pluginState;
        //pluginState = xmlState;
        String tmpValue;

        tmpValue = pluginState->getStringAttribute("xmlFilePath", "");
        if (tmpValue != "") {
            debug("restore state, load Xml File by Path: " + tmpValue);
            loadXmlFile(tmpValue);
        }

        delete pluginState;
    */
}


//==============================================================================

void JMidiTriggerAudioProcessor::setStateValue(juce::Identifier& key, const juce::String& value)
{
    /*XmlElement* valNode = pluginState->getChildByName(key);

    if (!valNode) {
        valNode = new XmlElement(key);
        pluginState->addChildElement(valNode);
    }

    valNode->setAttribute("value",value);
    delete valNode;*/
    //if (pluginState) pluginState->setAttribute(key, value);
}

juce::String JMidiTriggerAudioProcessor::getStateValue(juce::Identifier& key, const juce::String& defaultValue)
{
    /*XmlElement* valNode = pluginState->getChildByName(key);
    if (!valNode) return defaultValue;
    String result = valNode->getStringAttribute("value", defaultValue);
    delete valNode;
    return result;*/
    //if (pluginState) return pluginState->getStringAttribute(key, defaultValue);
    //else return defaultValue;
    return defaultValue;
}

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

	if (message.isNoteOn())
	{
		type = "noteon";
		key = message.getNoteNumber();
	}
	else if (message.isNoteOff())
	{
		type = "noteoff";
		key = message.getNoteNumber();
	}
	else if (message.isController())
	{
		type = "cc";
		key = message.getControllerNumber();
	}
	else if (message.isProgramChange())
	{
		type = "pc";
		key = message.getProgramChangeNumber();
	}

	struct result { juce::String type; int key; };
	return result{ type, key };

}

pugi::xpath_variable_set JMidiTriggerAudioProcessor::createXMLListenerQueryParams(int channel, int key, juce::String type) {
	pugi::xpath_variable_set params;

	params.add("channel", pugi::xpath_value_type::xpath_type_number);
	params.add("type", pugi::xpath_value_type::xpath_type_string);
	params.add("key", pugi::xpath_value_type::xpath_type_number);

	params.set("channel", double(channel));
	params.set("type", type.getCharPointer());
	params.set("key", double(key));

	return params;
}

auto JMidiTriggerAudioProcessor::createMidiMessage(MidiMessageInfo info)
{
	MidiMessage outMidiMsg;

	if (info.type == "noteon") {
		outMidiMsg = MidiMessage::noteOn(info.channel, info.key, float(info.value));
	}
	else if (info.type == "noteoff") {
		outMidiMsg = MidiMessage::noteOff(info.channel, info.key, float(info.value));
	}
	else if (info.type == "cc") {
		outMidiMsg = MidiMessage::controllerEvent(info.channel, info.key, info.value);
	}
	else if (info.type == "pc") {
		outMidiMsg = MidiMessage::programChange(info.channel, info.key);
	}

	return outMidiMsg;
}



bool JMidiTriggerAudioProcessor::processMidiInputMessage(const juce::MidiMessage& message, juce::MidiBuffer& midiOutput)
{
	//<listener channel = "2" type = "cc" key = "11" >
	//const MidiMessage m = MidiMessage::noteOn(message.getChannel(), message.getNoteNumber(), newVel);

	auto [type, key] = getMidiMessageTypeAndKey(message);
	XMLReader& xmlReader = XMLReader::getInstance();
	int channel = message.getChannel();

	pugi::xpath_variable_set params = createXMLListenerQueryParams(channel, key, type);

	logger.debug("search listener for type=" + juce::String(type) + " channel=" + juce::String(channel) + " key=" + juce::String(key));

	bool foundAnyData = false;
	/**
	  Reminder: we are searching for 
	  - a listener (=targetNode) 
	  - that contains IDs pointing to one or multiple events by ID
	  - each of those events can contain multiple instructions on to-be-sent data.
	  */
	// [@type=string(type) or @type=all]
	//pugi::xpath_query midiListenerQuery(listenerQuery);
	pugi::xpath_query midiListenerQuery("listener[@channel=number($channel)][@key=number($key)][@type=string($type) or @type='all']", &params);
	pugi::xpath_node targetNode = midiListenerQuery.evaluate_node(xmlReader.parser->xmlListenersNode);
	Array<pugi::string_t> eventIds = xmlReader.parser->getEventIdsForListener(&targetNode.node());

	if (targetNode) {

		if (type == "noteoff" && targetNode.node().attribute("type").as_string() == "all") {
			// ignore noteoff midi input for type "all" listeners
			return false;
		}

		logger.debug("midi event found: " + String(targetNode.node().name()));
		pugi::xml_node eventNode;
		pugi::xml_node midiNode;
		int sortIndex = 0;
		MidiMessage outMidiMsg;

		//debug("Listener has " + String(eventIds.size()) + " triggers assigned.");
		for (int i = 0; i < eventIds.size(); i++) {
			eventNode = xmlReader.parser->getEventNode(eventIds[i]);
			logger.debug("Listener trigger #" + String(i + 1) + ": " + String(eventIds[i].c_str()) + " node found: " + String(eventNode.name()));
			if (eventNode) {
				midiNode = eventNode.child("midi");
				for (midiNode; midiNode; midiNode = midiNode.next_sibling("midi")) {
					sortIndex++;

					MidiMessageInfo info = xmlReader.parser->midiNodeToMidiMessageInfo(midiNode);
					outMidiMsg = createMidiMessage(info);

					logger.debug("Send midi " + String(info.type) + " @Ch " + String(info.channel) + " " + String(info.key) + "," + String(info.value));
					foundAnyData = true;
					midiOutput.addEvent(outMidiMsg, sortIndex);
				}
			}
		}
	}
	else
	{
		logger.debug("midi event has no listener assigned.");
	}
	return foundAnyData;
}





//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JMidiTriggerAudioProcessor();
}
