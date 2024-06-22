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


juce::String XMLReader::getRelativeFilePath(const juce::File& fi)
{
	// TODO: should add a depth limit... a rel path like ../../../../../../../ is not good.
	if (!fi.exists())
	{
		logger.log("Error - file does not exist: " + fi.getFullPathName(),1);
		return juce::String("");
	}
	else
	{
		const auto applicationDir = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentApplicationFile).getParentDirectory();
	 	juce::String relativeXmlFilePath = fi.getRelativePathFrom(applicationDir);
	 	//logger.log(">>> relative path to config file: " + relativeXmlFilePath,1);
	 	//setStateValue(Identifier("xmlFilePath"), xmlFilePath.getValue().toString());
	 	//debug("after setstatevalue");

		return relativeXmlFilePath;

	}
}

juce::String solveRelativeFilePath(const juce::String& path)
{
	if(juce::File::isAbsolutePath(path))
	{
		return path;
	}
	else
	{
		// to allow for use of both absolute and relative paths, first resolve working dir.
		// getChildFile will handle absolute paths as well, so no need to check if rel or abs.
		const auto applicationDir = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentApplicationFile).getParentDirectory();
		const auto& resolvedFile = applicationDir.getChildFile(path);
		return resolvedFile.getFullPathName();
	}
}

// overload: from juce::File
bool XMLReader::loadXmlFile(const juce::File& fi)
{
	if (!fi.exists())
	{
		return abortLoadXmlFile();
	}

	xmlFilePath.setValue(fi.getFullPathName());

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

// overload: from juce::String
bool XMLReader::loadXmlFile(const juce::String& filePath)
{
	const auto resolvedPath = solveRelativeFilePath(filePath);
	const auto& fi = juce::File(resolvedPath);

	logger.debug("load xml file " + resolvedPath, 1);
	
	return this->loadXmlFile(fi);
}


bool XMLReader::isReady() {
	if (!parser) return false;
	return parser->xmlReadyState;
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

