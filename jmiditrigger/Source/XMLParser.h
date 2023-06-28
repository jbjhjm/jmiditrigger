/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "StatusLog.h"
#include "MidiUtils.h"
#include "../libraries/pugixml.hpp" 

//==============================================================================
struct XmlEventInfo {
    juce::String name;
};

/**
*/
class XMLParser
{
public:
    //==============================================================================
    static XMLParser& getInstance();
    XMLParser();
    ~XMLParser();

    bool loadXmlData(pugi::xml_document* doc);
    MidiUtils::MidiMessageAttributes getMidiMessageAttributes(pugi::xml_node& midiNode);
    MidiUtils::MidiMessageInfo midiNodeToMidiMessageInfo(pugi::xml_node& midiNode, MidiUtils::MidiMessageInfo& inputInfo);
    pugi::xml_node getEventNode(pugi::string_t eventId);
    XmlEventInfo* getEventInfoFromXml(pugi::string_t eventId);
    juce::Array<pugi::string_t> getEventIdsForListener(const pugi::xml_node* listenerNode);
    juce::String generateXmlDocumentation();

    pugi::xml_node findListenerNode(pugi::xpath_variable_set* params);
    int countNodeChildren(pugi::xml_node& node, const char * name);

    pugi::xpath_variable_set createListenerQueryParams(int channel, int key, juce::String type);
    bool handleMidiEvent(MidiUtils::MidiMessageInfo& inputInfo, juce::MidiBuffer& midiOutput);
    bool sendResponseForMidiEvent(pugi::xml_node& listenerNode, MidiUtils::MidiMessageInfo& inputInfo, juce::MidiBuffer& midiOutput);
    void generateOutputFromMidiNode(pugi::xml_node& midiNode, MidiUtils::MidiMessageInfo& inputInfo, juce::MidiBuffer& midiOutput, int midiEventIndex);

    StatusLog logger;// = StatusLog::getInstance();

    pugi::xml_document* xmlDoc;
    pugi::xml_node xmlRootNode;
    pugi::xml_node xmlEventsNode;
    pugi::xml_node xmlVarsNode;
    pugi::xml_node xmlListenersNode;
    bool xmlReadyState = false;


private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(XMLParser)
};
