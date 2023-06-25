/*
  ==============================================================================

    StatusLog.cpp
    Created: 25 Jun 2023 7:16:07pm
    Author:  Jannik

  ==============================================================================
*/

#pragma once

//#include "StatusLog.h"
#include <JuceHeader.h>

class StatusLog
{
private:
	juce::String logData;

public:



	static StatusLog& getInstance()
	{
		static StatusLog instance;
		return instance;
	}

	StatusLog()
	{
	}

	~StatusLog()
	{
	}

	juce::String getLog()
	{
		return logData;
	}

	void logMidiMessage(const juce::String& txt)
	{
		//String tmp = statusLog.getValue().toString();
		//tmp.append("\n" + txt, 2000);
		logData += "\n" + txt;
	}

	void log(const juce::String& txt)
	{
		DBG(txt);
		logData += "\n" + txt;
	}

	void debug(const juce::String& txt)
	{
#ifdef _DEBUG
		DBG(txt);
		logData += "\n" + txt;
#endif
	}

};