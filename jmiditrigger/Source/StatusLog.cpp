/*
  ==============================================================================

    StatusLog.cpp
    Created: 25 Jun 2023 7:16:07pm
    Author:  Jannik

  ==============================================================================
*/

#pragma once

#include "IDs.cpp"
#include "Store.cpp"
#include <JuceHeader.h>

class StatusLog
{
	static const juce::Identifier logTreeIdentifier; // pre-create an Identifier

private:
	juce::String logData;

public:

	//https://forum.juce.com/t/understanding-valuetrees-better/32414/2

	juce::ValueTree logTree;


	static StatusLog& getInstance()
	{
		static StatusLog instance;
		return instance;
	}

	StatusLog()
	{
		logTree = Store::getState(STATES::Log);
		logTree.setProperty(PROPS::LogData, "", nullptr);
	}

	~StatusLog()
	{
	}

	juce::String getLog()
	{
		return logTree.getProperty(PROPS::LogData);
	}

	void addToLog(juce::String text) {
		const juce::String logData = getLog();
		logTree.setProperty(PROPS::LogData, logData+"\n"+text, nullptr);
	}

	/*void logMidiMessage(const juce::String& txt)
	{
		//String tmp = statusLog.getValue().toString();
		//tmp.append("\n" + txt, 2000);
		logData += "\n" + txt;
	}*/

	void log(const juce::String& txt)
	{
		DBG(txt);
		addToLog(txt);
	}

	void debug(const juce::String& txt)
	{
#ifdef _DEBUG
		DBG(txt);
		addToLog(txt);
#endif
	}



	void logMidiMessage(const juce::MidiMessage& message, const juce::String& source)
	{
		const double time = message.getTimeStamp();

		const int hours = ((int)(time / 3600.0)) % 24;
		const int minutes = ((int)(time / 60.0)) % 60;
		const int seconds = ((int)time) % 60;
		const int millis = ((int)(time * 1000.0)) % 1000;

		const juce::String timecode(juce::String::formatted("%02d:%02d:%02d.%03d", hours, minutes, seconds, millis));
		const juce::String description(getMidiMessageDescription(message));
		const juce::String midiMessageString(timecode + "  -  " + description + " (" + source + ")");

		log(midiMessageString);
	}

	static juce::String getMidiMessageDescription(const juce::MidiMessage& m)
	{

		juce::String type = "";
		int channel = 0;
		int key = 0;
		int val = 0;

		/*
		if (m.isNoteOn())           return "Note on " + MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3);
		if (m.isNoteOff())          return "Note off " + MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3);
		if (m.isProgramChange())    return "Program change " + String(m.getProgramChangeNumber());
		//if (m.isPitchWheel())       return "Pitch wheel " + String(m.getPitchWheelValue());
		//if (m.isAftertouch())       return "After touch " + MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3) + ": " + String(m.getAfterTouchValue());
		//if (m.isChannelPressure())  return "Channel pressure " + String(m.getChannelPressureValue());
		//if (m.isAllNotesOff())      return "All notes off";
		//if (m.isAllSoundOff())      return "All sound off";
		//if (m.isMetaEvent())        return "Meta event";
		if (m.isController())
		{
		String name(MidiMessage::getControllerName(m.getControllerNumber()));

		if (name.isEmpty())
		name = "[" + String(m.getControllerNumber()) + "]";

		return "Controller " + name + ": " + String(m.getControllerValue());
		}
		*/

		channel = m.getChannel();
		if (m.isNoteOn())
		{
			type = "Note On ";
			key = m.getNoteNumber();
			val = m.getVelocity();
		}
		else if (m.isNoteOff())
		{
			type = "Note Off ";
			key = m.getNoteNumber();
			val = m.getVelocity();
		}
		else if (m.isController())
		{
			type = "CC      ";
			key = m.getControllerNumber();
			val = m.getControllerValue();
		}
		else if (m.isProgramChange())
		{
			type = "PC      ";
			key = m.getProgramChangeNumber();
			val = 1;
		}

		return type + " [Ch " + juce::String(channel) + "] " + " (" + juce::String(key) + ", " + juce::String(val) + ") ";

	}

};

