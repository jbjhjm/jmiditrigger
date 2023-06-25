/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "XMLReader.h"

//==============================================================================
XMLReader::XMLReader()
{
	logger = StatusLog::getInstance();
}

XMLReader::~XMLReader()
{
}


bool XMLReader::loadXmlFile(const juce::File& fi)
{
	logger.log("Loading XML File " + fi.getFullPathName());
	xmlReadyState = false;
	if (!fi.exists())
	{
		logger.log("Error - file does not exist: " + fi.getFullPathName());
		xmlFilePath = "";
		abortLoadXmlFile();
		return false;
	}
	else
	{
	 	xmlFilePath = fi.getRelativePathFrom(juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentApplicationFile).getParentDirectory());
	 	logger.debug("debug: rel xmlFilePath = " + xmlFilePath.getValue().toString());
	 	//setStateValue(Identifier("xmlFilePath"), xmlFilePath.getValue().toString());
	 	//debug("after setstatevalue");

	 	pugi::xml_parse_result xmlReadSuccess = xmlDoc.load_file(fi.getFullPathName().toRawUTF8());

	 	if (xmlReadSuccess == false) {
	 		logger.log("Error while reading XML file: " + juce::String(xmlReadSuccess.description()));
	 		abortLoadXmlFile();
	 		return false;
	 	}
	 	else 
		{
	 		logger.log("Successfully parsed file: " + xmlFilePath.toString());
	 		if (!loadXmlData()) { abortLoadXmlFile(); return false; };
	 		xmlReadyState = true;
	 		//generateXmlDocumentation();
	 		return true;
	 	}
	}
}


bool XMLReader::loadXmlFile(const juce::String& filePath)
{
	// File(filePath) is only allowed for absolute paths!
	logger.debug("load xml file from rel string:" + juce::String(
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

void XMLReader::abortLoadXmlFile()
{
	xmlReadyState = false;
}

bool XMLReader::reloadFile()
{
	return this->loadXmlFile(xmlFilePath.toString());
}

bool XMLReader::loadXmlData()
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


