/*
  ==============================================================================

    MidiUtils.h
    Created: 28 Jun 2023 12:34:36am
    Author:  Jannik

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>




namespace MidiUtils {

	struct MidiMessageInfo {
		juce::String type;
		int channel;
		int key;
		int value;
	};

	juce::MidiMessage createMidiMessage(MidiMessageInfo info);


}

