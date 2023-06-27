/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "XMLReader.h"

XMLReader& XMLReader::getInstance()
{
	static XMLReader instance;
	return instance;
}

//==============================================================================
XMLReader::XMLReader()
{
	DBG(">>>>>>>>>>>>>XMLReader::XMLReader" );
	logger = StatusLog::getInstance();
	parser = &XMLParser::getInstance();
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
	 		if (!parser->loadXmlData(&xmlDoc)) { 
				abortLoadXmlFile(); 
				return false; 
			};
	 		xmlReadyState = true;
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
	if (!xmlFilePath.getValue()) {
		logger.log("No config file is selected, cannot reload.");
		return false;
	}
	logger.log("Reloading config file...");
	return this->loadXmlFile(xmlFilePath.toString());
}

