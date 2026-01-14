#pragma once

#include "MidiUtils.h"




namespace MidiUtils {



	juce::MidiMessage createMidiMessage(MidiMessageInfo info)
	{
		juce::MidiMessage outMidiMsg;

		if (info.type == "noteon") {
			outMidiMsg = juce::MidiMessage::noteOn(info.channel, info.key, float(info.value) / 127.f); // float - 0 < velocity < 1
		}
		else if (info.type == "noteoff") {
			outMidiMsg = juce::MidiMessage::noteOff(info.channel, info.key, float(info.value) / 127.f);
		}
		else if (info.type == "cc") {
			outMidiMsg = juce::MidiMessage::controllerEvent(info.channel, info.key, info.value);
		}
		else if (info.type == "pc") {
			outMidiMsg = juce::MidiMessage::programChange(info.channel, info.key);
		}

		return outMidiMsg;
	}

	juce::String getPropFromMidiNodeAttributes(juce::String varName, MidiMessageAttributes& attr)
	{
		juce::String result;
		if (varName == "value") { result = attr.value; }
		else if (varName == "channel") { result = attr.channel; }
		else if (varName == "key") { result = attr.key; }
		else if (varName == "type") { result = attr.type; }
		else { throw std::exception("Unknown reference requested "); }
		return result;
	}

	int getPropFromMidiMessageInfo(juce::String varName, MidiMessageInfo info)
	{
		if (varName == "value") { return info.value; }
		else if (varName == "channel") { return info.channel; }
		else if (varName == "key") { return info.key; }
		else if (varName == "type") { throw std::exception("getPropFromMidiMessageInfo cannot be used to get type! (only int props supported)"); }
		else { throw std::exception("Unknown reference requested "); }
	}

	void setPropInMidiMessageInfo(juce::String varName, int value, MidiMessageInfo& info)
	{
		if (varName == "value") { info.value = value; }
		else if (varName == "channel") { info.channel = value; }
		else if (varName == "key") { info.key = value; }
		else if (varName == "type") { throw std::exception("getPropFromMidiMessageInfo cannot be used to set type! (only int props supported)"); }
		else { throw std::exception("Unknown reference requested "); }
	}


}









