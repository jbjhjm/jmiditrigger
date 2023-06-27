#pragma once

#include "MidiUtils.h"




namespace MidiUtils {



	juce::MidiMessage createMidiMessage(MidiMessageInfo info)
	{
		juce::MidiMessage outMidiMsg;

		if (info.type == "noteon") {
			outMidiMsg = juce::MidiMessage::noteOn(info.channel, info.key, float(info.value));
		}
		else if (info.type == "noteoff") {
			outMidiMsg = juce::MidiMessage::noteOff(info.channel, info.key, float(info.value));
		}
		else if (info.type == "cc") {
			outMidiMsg = juce::MidiMessage::controllerEvent(info.channel, info.key, info.value);
		}
		else if (info.type == "pc") {
			outMidiMsg = juce::MidiMessage::programChange(info.channel, info.key);
		}

		return outMidiMsg;
	}


}









