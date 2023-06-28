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
	xmlFilePath = "";
}

XMLReader::~XMLReader()
{
}


bool XMLReader::loadXmlFile(const juce::File& fi)
{
	logger.log("Loading XML File " + fi.getFullPathName());
	if (!fi.exists())
	{
		logger.log("Error - file does not exist: " + fi.getFullPathName(),1);
		xmlFilePath = "";
		return abortLoadXmlFile();
	}
	else
	{
	 	xmlFilePath = fi.getRelativePathFrom(juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentApplicationFile).getParentDirectory());
	 	logger.log("relative path to config file: " + xmlFilePath.getValue().toString(),1);
	 	//setStateValue(Identifier("xmlFilePath"), xmlFilePath.getValue().toString());
	 	//debug("after setstatevalue");

	 	pugi::xml_parse_result xmlReadSuccess = xmlDoc.load_file(fi.getFullPathName().toRawUTF8());

	 	if (xmlReadSuccess == false) {
	 		logger.log("Error while reading file: " + juce::String(xmlReadSuccess.description()),1);
			return abortLoadXmlFile();
		}
	 	else 
		{
	 		logger.log("Successfully parsed file." ,1);
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
	const auto currentDir = juce::File::getSpecialLocation(
		juce::File::SpecialLocationType::currentApplicationFile
	).getParentDirectory();

	// File(filePath) is only allowed for absolute paths!
	logger.debug("load xml file (baseDir, relPath):", 1);
	logger.debug(juce::String(currentDir.getFullPathName()), 2);
	logger.debug(filePath, 2);
		
	return this->loadXmlFile(currentDir.getChildFile(filePath));
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
	const auto filePath = xmlFilePath.getValue().toString();
	if (filePath == "") {
		logger.log("No config file is selected, cannot reload.");
		return false;
	}
	logger.log("Reloading config file...");
	return this->loadXmlFile(xmlFilePath.toString());
}

