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
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
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
        addMidiMessageToList(m);
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
	log("Loading XML File " + fi.getFullPathName());
	xmlReadyState = false;
	if (!fi.exists())
	{
		log("Error - file does not exist: " + fi.getFullPathName());
		xmlFilePath = "";
		abortLoadXmlFile();
		return false;
	}
	else
	{
	 	xmlFilePath = fi.getRelativePathFrom(juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentApplicationFile).getParentDirectory());
	 	debug("debug: rel xmlFilePath = " + xmlFilePath.getValue().toString());
	 	//setStateValue(Identifier("xmlFilePath"), xmlFilePath.getValue().toString());
	 	//debug("after setstatevalue");

	 	pugi::xml_parse_result xmlReadSuccess = xmlDoc.load_file(fi.getFullPathName().toRawUTF8());

	 	if (xmlReadSuccess == false) {
	 		log("Error while reading XML file: " + juce::String(xmlReadSuccess.description()));
	 		abortLoadXmlFile();
	 		return false;
	 	}
	 	else 
		{
	 		log("Successfully parsed file: " + xmlFilePath.toString());
	 		if (!loadXmlData()) { abortLoadXmlFile(); return false; };
	 		xmlReadyState = true;
	 		generateXmlDocumentation();
	 		return true;
	 	}
	}
}


bool JMidiTriggerAudioProcessor::loadXmlFile(const juce::String& filePath)
{
	// File(filePath) is only allowed for absolute paths!
	debug("load xml file from rel string:" + juce::String(
		juce::File::getSpecialLocation(
			juce::File::SpecialLocationType::currentApplicationFile
		).getParentDirectory()
		 .getFullPathName()
	) + ", " + filePath);
	return this->loadXmlFile(
		juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentApplicationFile)
			.getParentDirectory()
			.getChildFile(filePath)
	);
}

void JMidiTriggerAudioProcessor::abortLoadXmlFile()
{
	xmlReadyState = false;
}

bool JMidiTriggerAudioProcessor::reloadFile()
{
	return this->loadXmlFile(xmlFilePath.toString());
}

bool JMidiTriggerAudioProcessor::loadXmlData()
{
	xmlRootNode = xmlDoc.document_element();
	if (!xmlRootNode) { DBG("Error: No XML root node found. "); return false; }
	DBG("Debug: Selected root node " + juce::String(xmlRootNode.name()));

	xmlEventsNode = xmlRootNode.child("events");
	if (!xmlEventsNode) { DBG("Error: No XML <events> node found. "); return false; }
	DBG("Debug: Selected events group node " + juce::String(xmlEventsNode.name()));

	xmlListenersNode = xmlRootNode.child("listeners");
	if (!xmlListenersNode) { DBG("Error: No XML <listeners> node found. "); return false; }
	DBG("Debug: Selected listeners group node " + juce::String(xmlListenersNode.name()));

	return true;
}

void JMidiTriggerAudioProcessor::generateXmlDocumentation()
{
	// if (!xmlReadyState) return;
	// log("Debug: Generate documentation");

	// juce::String doc = "";
	// //pugi::xml_node listenerNode = xmlListenersNode->child("listener");
	// pugi::xml_node eventNode;
	// juce::Array<pugi::string_t> eventIds;
	// pugi::string_t eventName;

	// //DBG("Debug: Selected events group node " );

	// for (pugi::xml_node listenerNode = xmlListenersNode.child("listener"); listenerNode; listenerNode = listenerNode.next_sibling("listener")) {
	// 	DBG("Debug: Found a listener node");
	// 	doc +=
	// 		"Listener at Channel " + juce::String(listenerNode.attribute("channel").as_string()) +
	// 		" " + juce::String(listenerNode.attribute("type").as_string()) +
	// 		" [ " + juce::String(listenerNode.attribute("key").as_string()) + 
	// 		" " + juce::String(listenerNode.attribute("value").as_string()) + 
	// 		" ] " +
	// 		" ";

	// 	eventIds = getEventIdsForListener(&listenerNode);
	// 	if (eventIds.size() == 0) {
	// 		doc += "\t Listener triggers nothing \n";
	// 	}
	// 	else {
	// 		//doc += "\tListener has " + String(eventIds.size()) + " triggers assigned. \n";
	// 		if (eventIds.size() > 1) {
	// 			doc += "\n";
	// 		}
	// 		for (int i = 0; i < eventIds.size(); i++) {
	// 			eventNode = xmlEventsNode.find_child_by_attribute("event", "id", eventIds[i].c_str());
	// 			if (eventNode) {
	// 				eventName = eventNode.attribute("name").as_string("");
	// 				if (eventName == "") eventName = eventIds[i].c_str();
	// 				doc += "\tEvent #" + juce::String(i + 1) + " - " + eventName + "\n";
	// 			}
	// 			else {
	// 				doc += "\tEvent Node '" + juce::String(eventIds[i].c_str()) + "' not found. \n";
	// 			}
	// 		}
	// 	}
	// }

	// midiDataInfo = doc;
	// DBG("Successfully parsed file.");
}

static juce::String getMidiMessageDescription(const juce::MidiMessage& m)
{

	juce::String type = "";
	int channel = 0;
	int key = 0;
	int val = 0;

	/*
	if (m.isNoteOn())           return "Note on " + MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3);
	if (m.isNoteOff())          return "Note off " + MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3);
	if (m.isProgramChange())    return "Program change " + String(m.getProgramChangeNumber());
	//if (m.isPitchWheel())       return "Pitch wheel " + String(m.getPitchWheelValue());
	//if (m.isAftertouch())       return "After touch " + MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3) + ": " + String(m.getAfterTouchValue());
	//if (m.isChannelPressure())  return "Channel pressure " + String(m.getChannelPressureValue());
	//if (m.isAllNotesOff())      return "All notes off";
	//if (m.isAllSoundOff())      return "All sound off";
	//if (m.isMetaEvent())        return "Meta event";
	if (m.isController())
	{
	String name(MidiMessage::getControllerName(m.getControllerNumber()));

	if (name.isEmpty())
	name = "[" + String(m.getControllerNumber()) + "]";

	return "Controller " + name + ": " + String(m.getControllerValue());
	}
	*/

	channel = m.getChannel();
	if (m.isNoteOn())
	{
		type = "Note On ";
		key = m.getNoteNumber();
		val = m.getVelocity();
	}
	else if (m.isNoteOff())
	{
		type = "Note Off ";
		key = m.getNoteNumber();
		val = m.getVelocity();
	}
	else if (m.isController())
	{
		type = "CC      ";
		key = m.getControllerNumber();
		val = m.getControllerValue();
	}
	else if (m.isProgramChange())
	{
		type = "PC      ";
		key = m.getProgramChangeNumber();
		val = 1;
	}

	return type + " [Ch " + juce::String(channel) + "] " + " (" + juce::String(key) + ", " + juce::String(val) + ") ";

}

void JMidiTriggerAudioProcessor::addMidiMessageToList(const juce::MidiMessage& message, const juce::String& source)
{
	const double time = message.getTimeStamp();

	const int hours = ((int)(time / 3600.0)) % 24;
	const int minutes = ((int)(time / 60.0)) % 60;
	const int seconds = ((int)time) % 60;
	const int millis = ((int)(time * 1000.0)) % 1000;

	const juce::String timecode(juce::String::formatted("%02d:%02d:%02d.%03d", hours, minutes, seconds, millis));
	const juce::String description(getMidiMessageDescription(message));
	const juce::String midiMessageString(timecode + "  -  " + description + " (" + source + ")");

	log(midiMessageString);
}


bool JMidiTriggerAudioProcessor::processMidiInputMessage(const juce::MidiMessage& message, juce::MidiBuffer& midiOutput)
{
	return false;
	//<listener channel = "2" type = "cc" key = "11" >
	//m = MidiMessage::noteOn(m.getChannel(), m.getNoteNumber(), newVel);
	// juce::String type = "";
	// int channel = 0;
	// int key = 0;

	// pugi::xpath_variable_set params;
	// bool foundAnyData = false;

	// params.add("channel", pugi::xpath_value_type::xpath_type_number);
	// params.add("type", pugi::xpath_value_type::xpath_type_string);
	// params.add("key", pugi::xpath_value_type::xpath_type_number);

	// channel = message.getChannel();
	// if (message.isNoteOn())
	// {
	// 	type = "noteon";
	// 	key = message.getNoteNumber();
	// }
	// else if (message.isNoteOff())
	// {
	// 	type = "noteoff";
	// 	key = message.getNoteNumber();
	// }
	// else if (message.isController())
	// {
	// 	type = "cc";
	// 	key = message.getControllerNumber();
	// }
	// else if (message.isProgramChange())
	// {
	// 	type = "pc";
	// 	key = message.getProgramChangeNumber();
	// }

	// params.set("channel", double(channel));
	// params.set("type", type.getCharPointer());
	// params.set("key", double(key));

	// debug("search listener for type=" + juce::String(type) + " channel=" + juce::String(channel) + " key=" + juce::String(key));

	// // [@type=string(type) or @type=all]
	// pugi::xpath_query midiListenerQuery("listener[@channel=number($channel)][@key=number($key)][@type=string($type) or @type='all']", &params);
	// pugi::xpath_node targetNode = midiListenerQuery.evaluate_node(xmlListenersNode);
	// if (targetNode) {

	// 	if (type == "noteoff" && targetNode.node().attribute("type").as_string() == "all") {
	// 		// ignore noteoff midi input for type "all" listeners
	// 		return false;
	// 	}

	// 	debug("midi event found: " + String(targetNode.node().name()));
	// 	pugi::xml_node eventNode;
	// 	pugi::xml_node midiNode;
	// 	pugi::string_t outType;
	// 	int outChannel;
	// 	int outKey;
	// 	int outVal;
	// 	int sortIndex = 0;
	// 	Array<pugi::string_t> eventIds = getEventIdsForListener(&targetNode.node());
	// 	MidiMessage outMidiMsg;

	// 	//debug("Listener has " + String(eventIds.size()) + " triggers assigned.");
	// 	for (int i = 0; i < eventIds.size(); i++) {
	// 		eventNode = xmlEventsNode.find_child_by_attribute("event", "id", eventIds[i].c_str());
	// 		debug("Listener trigger #" + String(i + 1) + ": " + String(eventIds[i].c_str()) + " node found: " + String(eventNode.name()));
	// 		if (eventNode) {
	// 			midiNode = eventNode.child("midi");
	// 			for (midiNode; midiNode; midiNode = midiNode.next_sibling("midi")) {
	// 				sortIndex++;
	// 				outChannel = midiNode.attribute("channel").as_int(1);
	// 				outType = midiNode.attribute("type").as_string("");
	// 				outKey = midiNode.attribute("key").as_int(0);
	// 				outVal = midiNode.attribute("value").as_int(0);
	// 				debug("Send midi " + String(outType) + " @Ch " + String(outChannel) + " " + String(outKey) + "," + String(outVal));
	// 				if (outType == "noteon") {
	// 					outMidiMsg = MidiMessage::noteOn(outChannel, outKey, float(outVal));
	// 				}
	// 				else if (outType == "noteoff") {
	// 					outMidiMsg = MidiMessage::noteOff(outChannel, outKey, float(outVal));
	// 				}
	// 				else if (outType == "cc") {
	// 					outMidiMsg = MidiMessage::controllerEvent(outChannel, outKey, outVal);
	// 				}
	// 				else if (outType == "pc") {
	// 					outMidiMsg = MidiMessage::programChange(outChannel, outKey);
	// 				}
	// 				foundAnyData = true;
	// 				midiOutput.addEvent(outMidiMsg, sortIndex);
	// 			}
	// 		}
	// 		else
	// 		{
	// 			//doc += "\tEvent Node '" + String(eventIds[i].c_str()) + "' not found. \n";
	// 		}
	// 	}
	// }
	// else
	// {
	// 	debug("midi event has no listener assigned.");
	// }
	// return foundAnyData;
}

void JMidiTriggerAudioProcessor::logMidiMessage(const juce::String& txt)
{
	//String tmp = statusLog.getValue().toString();
	//tmp.append("\n" + txt, 2000);
	statusLog += "\n" + txt;
}

void JMidiTriggerAudioProcessor::log(const juce::String& txt)
{
	DBG(txt);
	statusLog += "\n" + txt;
}

void JMidiTriggerAudioProcessor::debug(const juce::String& txt)
{
#ifdef _DEBUG
	DBG(txt);
	statusLog += "\n" + txt;
#endif
}


juce::Array<pugi::string_t> JMidiTriggerAudioProcessor::getEventIdsForListener(const pugi::xml_node* listenerNode)
{
	juce::Array<pugi::string_t> EventIds;
	// pugi::string_t eventId;
	// pugi::xml_node triggerNode;

	// // trigger attribute shortcut
	// eventId = listenerNode->attribute("trigger").as_string("");
	// if (eventId != "") EventIds.add(eventId);

	// triggerNode = listenerNode->child("trigger");
	// //log("found a trigger node with tag " + String(triggerNode.name()) );
	// for (triggerNode; triggerNode; triggerNode = triggerNode.next_sibling("trigger")) {
	// 	eventId = triggerNode.attribute("id").as_string("");
	// 	//log("<trigger> with id "+eventId);
	// 	if (eventId != "") EventIds.add(eventId);
	// }

	return EventIds;
}



//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JMidiTriggerAudioProcessor();
}
