/*
  ==============================================================================

	StatusLog.cpp
	Created: 25 Jun 2023 7:16:07pm
	Author:  Jannik

  ==============================================================================

  Use getState with one of the Identifiers found in IDS.
  It will return the state, creating it on the fly if needed.
  As this is a constant class, no constructor passing is required.

  LOG state is temporary.
  CONFIG state will be persisted using hooks in PluginProcessor.
*/

#pragma once

#include "IDs.cpp"
#include <JuceHeader.h>

using namespace juce;

// https://forum.juce.com/t/valuetree-callback-when-root-node-changed/37846/11
/*void syncValueTreeNotifyListeners(const juce::ValueTree& source, juce::ValueTree& destination)
{
	const int numProperties = source.getNumProperties();
	for (int i = 0; i < numProperties; ++i)
	{
		auto propertyName = source.getPropertyName(i);

		if (destination.hasProperty(propertyName))
			destination.setProperty(propertyName, source.getProperty(propertyName), nullptr);
	}

	for (const auto& child : source)
	{
		auto childType = child.getType();
		auto childInDestination = destination.getChildWithName(childType);

		if (childInDestination.isValid())
			syncValueTreeNotifyListeners(child, childInDestination);
	}
}*/

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

	static ValueTree importToState(Identifier id, juce::XmlElement* xmlData) {

		auto& state = Store::getRoot().getChildWithName(id);

		if (xmlData != nullptr) {

			// manually send a change event for all children.
			// other solutions may be to replace or redirect the config state,
			// described here: 
			// https://forum.juce.com/t/add-notification-type-to-valuetrees/57145/9
			// https://forum.juce.com/t/valuetree-callback-when-root-node-changed/37846/11

			// fromXml returns a new state instance
			auto importedDataState = ValueTree::fromXml(*xmlData);

			// listeners not called!
			//state.copyPropertiesAndChildrenFrom(importedDataState, nullptr);
			//for (const auto& child : importedDataState) {
			//	state.sendPropertyChangeMessage(child.getType());
			//}

			// manually sync imported properties.
			// this should take care of firing listener events correctly.
			// note that only properties are handled, children ValueTrees are skipped!
			const int numProperties = importedDataState.getNumProperties();
			for (int i = 0; i < numProperties; ++i)
			{
				auto propertyName = importedDataState.getPropertyName(i);
				state.setProperty(propertyName, importedDataState.getProperty(propertyName), nullptr);
			}

			DBG(">>> imported state data:");
			DBG(state.toXmlString());

		}

		return state;
	}

	

private:

	static ValueTree addState(Identifier id) {
		const ValueTree newState = ValueTree{ id };
		Store::getRoot().addChild(newState, 0, nullptr);
		return newState;
	}




	

};

