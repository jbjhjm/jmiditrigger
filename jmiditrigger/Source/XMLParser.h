/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "StatusLog.cpp"
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
    MidiUtils::MidiMessageInfo midiNodeToMidiMessageInfo(pugi::xml_node& midiNode);
    pugi::xml_node getEventNode(pugi::string_t eventId);
    XmlEventInfo* getEventInfoFromXml(pugi::string_t eventId);
    juce::Array<pugi::string_t> getEventIdsForListener(const pugi::xml_node* listenerNode);
    juce::String generateXmlDocumentation();

    pugi::xpath_node* findListenerNode(pugi::xpath_variable_set* params);

    pugi::xpath_variable_set createListenerQueryParams(int channel, int key, juce::String type);
    bool handleMidiEvent(int channel, int key, juce::String type, juce::MidiBuffer& midiOutput);
    bool sendResponseForMidiEvent(pugi::xpath_node* targetNode, juce::MidiBuffer& midiOutput);
    void generateOutputFromMidiNode(pugi::xml_node& midiNode, juce::MidiBuffer& midiOutput, int midiEventIndex);

    StatusLog logger;// = StatusLog::getInstance();

    pugi::xml_document* xmlDoc;
    pugi::xml_node xmlRootNode;
    pugi::xml_node xmlEventsNode;
    pugi::xml_node xmlListenersNode;
    bool xmlReadyState = false;


private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(XMLParser)
};
