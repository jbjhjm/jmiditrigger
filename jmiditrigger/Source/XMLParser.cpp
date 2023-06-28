/*
  ==============================================================================

    XMLParser.cpp
    Created: 27 Jun 2023 10:22:37am
    Author:  Jannik

  ==============================================================================
*/
/**
  Reminder on XML features
  - <listener> listens to a certain kind of midi input (=targetNode)
  - <midi> provides information for a midi output
  - <event> groups a number of <midi> nodes and allow them to be referred to by <listener trigger="eventId">
  - new in v2: <listener> may contain <midi> children, 
    in this case these will generate output and check for triggered events will be skipped
  - TODO: simple variables? e.g. $axe3channel?
  - TODO: map input key/value to output - @value, @key
  */

#include "XMLParser.h"

XMLParser& XMLParser::getInstance()
{
	static XMLParser instance;
	return instance;
}

//==============================================================================
XMLParser::XMLParser():
	logger (StatusLog::getInstance())
{
	DBG(">>>>>>>>>>>>>XMLParser::XMLParser");
}

XMLParser::~XMLParser()
{
}


bool XMLParser::loadXmlData(pugi::xml_document* doc)
{
	xmlDoc = doc;
	xmlRootNode = xmlDoc->document_element();
	if (!xmlRootNode) { DBG("Error: No XML root node found. "); return false; }
	DBG("Debug: Selected root node " + juce::String(xmlRootNode.name()));

	xmlEventsNode = xmlRootNode.child("events");
	//if (!xmlEventsNode) { DBG("Error: No XML <events> node found. "); return false; }
	DBG("Debug: Selected events group node " + juce::String(xmlEventsNode.name()));

	xmlVarsNode = xmlRootNode.child("variables");

	xmlListenersNode = xmlRootNode.child("listeners");
	if (!xmlListenersNode) { DBG("Error: No XML <listeners> node found. "); return false; }
	DBG("Debug: Selected listeners group node " + juce::String(xmlListenersNode.name()));

	return true;
}

pugi::xpath_variable_set XMLParser::createListenerQueryParams(int channel, int key, juce::String type) {
	pugi::xpath_variable_set params;

	params.add("channel", pugi::xpath_value_type::xpath_type_number);
	params.add("type", pugi::xpath_value_type::xpath_type_string);
	params.add("key", pugi::xpath_value_type::xpath_type_number);

	params.set("channel", double(channel));
	params.set("type", type.getCharPointer());
	params.set("key", double(key));

	logger.debug("search listener for type=" + juce::String(type) + " channel=" + juce::String(channel) + " key=" + juce::String(key));

	return params;
}

pugi::xpath_node* XMLParser::findListenerNode(pugi::xpath_variable_set* params)
{
	pugi::xpath_query midiListenerQuery("listener[@channel=number($channel)][@key=number($key)][@type=string($type) or @type='all']", params);
	pugi::xpath_node targetNode = midiListenerQuery.evaluate_node(xmlListenersNode);
	return &targetNode;
}

MidiUtils::MidiMessageAttributes XMLParser::getMidiMessageAttributes(pugi::xml_node& midiNode)
{
	MidiUtils::MidiMessageAttributes info = {
		juce::String(midiNode.attribute("type").as_string()),
		juce::String(midiNode.attribute("channel").as_string()),
		juce::String(midiNode.attribute("key").as_string()),
		juce::String(midiNode.attribute("value").as_string())
	};
	return info;
}

MidiUtils::MidiMessageInfo XMLParser::midiNodeToMidiMessageInfo(pugi::xml_node & midiNode, MidiUtils::MidiMessageInfo & inputInfo)
{
	auto& midiNodeAttr = getMidiMessageAttributes(midiNode);

	MidiUtils::MidiMessageInfo info = {
		midiNodeAttr.type,
		0,
		0,
		0
	};

	std::map<juce::String, int> data;
	const pugi::char_t* keys[]{ "channel","key","value" };

	for (auto& key : keys) {
		auto strVal = MidiUtils::getPropFromMidiNodeAttributes(key, midiNodeAttr);
		bool isVar_dollar = strVal.startsWithChar(char("$"));
		bool isVar_at = strVal.startsWithChar(char("@"));
		if (isVar_dollar || isVar_at) {
			auto varName = strVal.substring(1);
			int value;
			if (isVar_dollar) {
				throw std::exception("Variables are not implemented yet");
				auto varNode = xmlVarsNode.child(varName.getCharPointer());
				value = varNode.attribute("value").as_int();
			}
			else 
			{
				value = MidiUtils::getPropFromMidiMessageInfo(varName, inputInfo);
			}
			data[key] = value;
		}
		else 
		{
			data[key] = strVal.getIntValue();
		}
	}

	return info;
}

pugi::xml_node XMLParser::getEventNode(pugi::string_t eventId)
{
	pugi::xml_node eventNode = xmlEventsNode.find_child_by_attribute("event", "id", eventId.c_str());
	return eventNode;
}

XmlEventInfo* XMLParser::getEventInfoFromXml(pugi::string_t eventId)
{
	pugi::xml_node eventNode = getEventNode(eventId);

	if (eventNode) {

		pugi::string_t eventName = eventNode.attribute("name").as_string("");
		if (eventName == "") eventName = eventId.c_str();

		XmlEventInfo* info = new XmlEventInfo{
			eventName
		};

		return info;
	}
	else {
		return NULL;
	}
}

bool XMLParser::handleMidiEvent(MidiUtils::MidiMessageInfo& inputInfo, juce::MidiBuffer & midiOutput)
{
	// [@type=string(type) or @type=all]
	//pugi::xpath_query midiListenerQuery(listenerQuery);

	pugi::xpath_variable_set params = createListenerQueryParams(inputInfo.channel, inputInfo.key, inputInfo.type);
	pugi::xpath_node* targetNode = findListenerNode(&params);

	if (targetNode) {
		if (inputInfo.type == "noteoff" && targetNode->node().attribute("type").as_string() == "all") {
			// special case: skip handling of noteoff midi event for type "all" listeners
			return true;
		}
		return sendResponseForMidiEvent(targetNode, inputInfo, midiOutput);
	}
	else 
	{
		return false;
	}
}

bool XMLParser::sendResponseForMidiEvent(pugi::xpath_node* targetNode, MidiUtils::MidiMessageInfo& inputInfo, juce::MidiBuffer& midiOutput) {
	
	logger.debug("send response as defined in found midi event: " + String(targetNode->node().name()));
	pugi::xml_node eventNode;
	pugi::xml_node midiNode;
	bool foundAnyData = false;
	int sortIndex = 0;
	MidiMessage outMidiMsg;

	Array<pugi::string_t> eventIds = getEventIdsForListener(&(targetNode->node()));
	midiNode = targetNode->node().child("midi");

	if (!midiNode.empty()) {
		// handle midi node stored as direct children
		for (midiNode; midiNode; midiNode = midiNode.next_sibling("midi")) {
			sortIndex++;
			foundAnyData = true;
			generateOutputFromMidiNode(midiNode, inputInfo, midiOutput, sortIndex);
		}
	}
	else if(!eventIds.isEmpty())
	{
		// handle midi nodes stored in events
		for (int i = 0; i < eventIds.size(); i++) {
			eventNode = getEventNode(eventIds[i]);
			logger.debug("Listener trigger #" + String(i + 1) + ": " + String(eventIds[i].c_str()) + " node found: " + String(eventNode.name()));
			if (eventNode) {
				midiNode = eventNode.child("midi");
				for (midiNode; midiNode; midiNode = midiNode.next_sibling("midi")) {
					sortIndex++;
					foundAnyData = true;
					generateOutputFromMidiNode(midiNode, inputInfo, midiOutput, sortIndex);
				}
			}
		}
	}
	else 
	{
		return false;
	}

	return foundAnyData;
}

void XMLParser::generateOutputFromMidiNode(pugi::xml_node& midiNode, MidiUtils::MidiMessageInfo& inputInfo, juce::MidiBuffer& midiOutput, int midiEventIndex)
{
	MidiUtils::MidiMessageInfo info = midiNodeToMidiMessageInfo(midiNode, inputInfo);
	logger.debug("Send midi " + String(info.type) + " @Ch " + String(info.channel) + " " + String(info.key) + "," + String(info.value));
	MidiMessage outMidiMsg = MidiUtils::createMidiMessage(info);
	midiOutput.addEvent(outMidiMsg, midiEventIndex);
}

juce::Array<pugi::string_t> XMLParser::getEventIdsForListener(const pugi::xml_node* listenerNode)
{
	juce::Array<pugi::string_t> EventIds;
	pugi::string_t eventId;
	pugi::xml_node triggerNode;

	// trigger attribute shortcut
	eventId = listenerNode->attribute("trigger").as_string("");
	if (eventId != "") EventIds.add(eventId);

	triggerNode = listenerNode->child("trigger");
	//log("found a trigger node with tag " + String(triggerNode.name()) );
	for (triggerNode; triggerNode; triggerNode = triggerNode.next_sibling("trigger")) {
		eventId = triggerNode.attribute("id").as_string("");
		//log("<trigger> with id "+eventId);
		if (eventId != "") EventIds.add(eventId);
	}

	return EventIds;
}

int XMLParser::countNodeChildren(pugi::xml_node& node, const char * name = "")
{
	if (name != "")
	{
		auto& childIterator = node.children(name);
		return std::distance(childIterator.begin(), childIterator.end());
	}
	else
	{
		auto& childIterator = node.children(name);
		return std::distance(childIterator.begin(), childIterator.end());
	}
}

juce::String XMLParser::generateXmlDocumentation()
{
	if (!xmlRootNode) return "no configuration loaded, aborting doc generation";
	logger.log("Debug: Generate documentation");

	juce::String doc = "";
	pugi::xml_node eventNode;
	juce::Array<pugi::string_t> eventIds;
	pugi::string_t eventName;

	//DBG("Debug: Selected events group node " );

	for (pugi::xml_node listenerNode = xmlListenersNode.child("listener"); listenerNode; listenerNode = listenerNode.next_sibling("listener")) {
		DBG("Debug: Found a listener node");

		MidiUtils::MidiMessageAttributes info = getMidiMessageAttributes(listenerNode);

		doc +=
			"Listener at Channel "
			+ juce::String(info.channel) +
			" [type=" + info.type +
			"] [ " + juce::String(info.key) + " " + juce::String(info.value) + " ] " +
			" ";

		if (!listenerNode.children("midi").empty()) {
			const int count = countNodeChildren(listenerNode, "midi");
			doc += "\t Listener executes "+(juce::String(count)) + " midi children \n";
		}
		else 
		{
			// search and list event
			eventIds = getEventIdsForListener(&listenerNode);
			if (eventIds.size() == 0) {
				doc += "\t Listener triggers nothing \n";
			}
			else
			{
				doc += "\tListener calls event(s): ";
				if (eventIds.size() > 1) {
					doc += "\n";
				}
				for (int i = 0; i < eventIds.size(); i++) {
					XmlEventInfo* eventInfo = getEventInfoFromXml(eventIds[i]);

					if (eventInfo) {
						DBG(eventInfo->name);
						doc += "\t" + juce::String(i + 1) + " - " + eventInfo->name + "\n";
					}
					else {
						doc += "\tEvent #'" + juce::String(eventIds[i].c_str()) + "' not found. \n";
					}
				}
			}
		}
	}

	//midiDataInfo = doc;
	DBG("Successfully parsed file.");
	return doc;
}
