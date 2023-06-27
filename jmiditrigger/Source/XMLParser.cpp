/*
  ==============================================================================

    XMLParser.cpp
    Created: 27 Jun 2023 10:22:37am
    Author:  Jannik

  ==============================================================================
*/

#include "XMLParser.h"

XMLParser& XMLParser::getInstance()
{
	static XMLParser instance;
	return instance;
}

//==============================================================================
XMLParser::XMLParser()
{
	DBG(">>>>>>>>>>>>>XMLParser::XMLParser");
	logger = StatusLog::getInstance();
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
	if (!xmlEventsNode) { DBG("Error: No XML <events> node found. "); return false; }
	DBG("Debug: Selected events group node " + juce::String(xmlEventsNode.name()));

	xmlListenersNode = xmlRootNode.child("listeners");
	if (!xmlListenersNode) { DBG("Error: No XML <listeners> node found. "); return false; }
	DBG("Debug: Selected listeners group node " + juce::String(xmlListenersNode.name()));

	return true;
}

MidiMessageInfo XMLParser::midiNodeToMidiMessageInfo(pugi::xml_node& midiNode)
{
	MidiMessageInfo info = {
		juce::String(midiNode.attribute("type").as_string()),
		midiNode.attribute("channel").as_int(1),
		midiNode.attribute("key").as_int(0),
		midiNode.attribute("value").as_int(0)
	};

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

		MidiMessageInfo info = midiNodeToMidiMessageInfo(listenerNode);

		doc +=
			"Listener at Channel "
			+ juce::String(info.channel) +
			" " + info.type +
			" [ " + juce::String(info.key) + " " + juce::String(info.value) + " ] " +
			" ";

		eventIds = getEventIdsForListener(&listenerNode);
		if (eventIds.size() == 0) {
			doc += "\t Listener triggers nothing \n";
		}
		else
		{
			//doc += "\tListener has " + String(eventIds.size()) + " triggers assigned. \n";
			if (eventIds.size() > 1) {
				doc += "\n";
			}
			for (int i = 0; i < eventIds.size(); i++) {
				XmlEventInfo* eventInfo = getEventInfoFromXml(eventIds[i]);

				if (eventInfo) {
					DBG(eventInfo->name);
					doc += "\tEvent #" + juce::String(i + 1) + " - " + eventInfo->name + "\n";
				}
				else {
					doc += "\tEvent Node '" + juce::String(eventIds[i].c_str()) + "' not found. \n";
				}
			}
		}
	}

	//midiDataInfo = doc;
	DBG("Successfully parsed file.");
	return doc;
}
