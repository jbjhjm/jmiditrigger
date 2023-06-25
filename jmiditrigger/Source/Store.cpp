/*
  ==============================================================================

	StatusLog.cpp
	Created: 25 Jun 2023 7:16:07pm
	Author:  Jannik

  ==============================================================================
*/

#pragma once

#include "IDs.cpp"
#include <JuceHeader.h>

using namespace juce;

class Store
{
public:

	static ValueTree& getRoot()
	{
		static ValueTree instance {IDs::ROOT_STORE};
		return instance;
	}

	static ValueTree getState(Identifier id) {
		const ValueTree& existing = Store::getRoot().getChildWithName(id);
		if (existing.isValid()) {
			return existing;
		}
		else {
			return addState(id);
		}
	}

private:

	static ValueTree addState(Identifier id) {
		const ValueTree newState = ValueTree{ id };
		Store::getRoot().addChild(newState, 0, nullptr);
		return newState;
	}




	

};

