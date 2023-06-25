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
/**
*/
class XMLReader 
{
public:
    //==============================================================================
    static XMLReader& getInstance();
    XMLReader();
    ~XMLReader();

    bool loadXmlFile(const juce::String& filePath);
    bool loadXmlFile(const juce::File& fi);
    void abortLoadXmlFile();
    bool reloadFile();
    bool loadXmlData();
    void generateXmlDocumentation();

    StatusLog logger;// = StatusLog::getInstance();

    juce::Value xmlFilePath;

    pugi::xml_document xmlDoc;
    pugi::xml_node xmlRootNode;
    pugi::xml_node xmlEventsNode;
    pugi::xml_node xmlListenersNode;
    bool xmlReadyState = false;


private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (XMLReader)
};
