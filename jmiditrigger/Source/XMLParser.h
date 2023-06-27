/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "StatusLog.cpp"
#include "../libraries/pugixml.hpp" 

//==============================================================================
struct MidiMessageInfo {
    juce::String type;
    int channel;
    int key;
    int value;
};
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
    MidiMessageInfo midiNodeToMidiMessageInfo(pugi::xml_node& midiNode);
    pugi::xml_node getEventNode(pugi::string_t eventId);
    XmlEventInfo* getEventInfoFromXml(pugi::string_t eventId);
    juce::Array<pugi::string_t> getEventIdsForListener(const pugi::xml_node* listenerNode);
    juce::String generateXmlDocumentation();

    StatusLog logger;// = StatusLog::getInstance();

    juce::Value documentation;

    pugi::xml_document* xmlDoc;
    pugi::xml_node xmlRootNode;
    pugi::xml_node xmlEventsNode;
    pugi::xml_node xmlListenersNode;
    bool xmlReadyState = false;


private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(XMLParser)
};
