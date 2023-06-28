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
XMLReader::XMLReader():
	logger (StatusLog::getInstance()),
	parser (&XMLParser::getInstance())
{
	documentation = "No config data loaded yet";
}

XMLReader::~XMLReader()
{
}


bool XMLReader::loadXmlFile(const juce::File& fi)
{
	logger.log("Loading XML File " + fi.getFullPathName());
	if (!fi.exists())
	{
		logger.log("Error - file does not exist: " + fi.getFullPathName());
		xmlFilePath = "";
		return abortLoadXmlFile();
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
			return abortLoadXmlFile();
		}
	 	else 
		{
	 		logger.log("Successfully parsed file: " + xmlFilePath.toString());
	 		if (!parser->loadXmlData(&xmlDoc)) { 
				return abortLoadXmlFile();
			};

			documentation = parser->generateXmlDocumentation();
			return completeLoadXmlFile();
		}
	}
}

bool XMLReader::isReady() {
	if (!parser) return false;
	return parser->xmlReadyState;
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

bool XMLReader::abortLoadXmlFile()
{
	//xmlReadyState = false;
	return false;
}

bool XMLReader::completeLoadXmlFile()
{
	//xmlReadyState = true;
	return true;
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

