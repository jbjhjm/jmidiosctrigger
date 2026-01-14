/*
  ==============================================================================

    StatusLog.cpp
    Created: 25 Jun 2023 7:16:07pm
    Author:  Jannik

  ==============================================================================
*/

#pragma once

#include "StatusLog.h"
#include "Constants.h"

juce::String makeIndentation(int indent)
{
	juce::String out = "";
	for (int repeat = 0; repeat < indent; repeat++)
	{
		out += "    ";
	}
	return out;
}




StatusLog& StatusLog::getInstance()
{
	static StatusLog instance;
	return instance;
}

StatusLog::StatusLog()
{
	logTree = Store::getState(STATES::Log);
	logTree.setProperty(PROPS::LogData, "", nullptr);
}

StatusLog::~StatusLog()
{
}

juce::String StatusLog::getLog()
{
	return logTree.getProperty(PROPS::LogData);
}

void StatusLog::addToLog(juce::String text, int indent) {
	juce::String existingLogData = getLog();

	// apply a simple trim solution based on char count in log
	const auto existingLogDataLength = existingLogData.length();
	if (existingLogDataLength > MAX_LOG_LENGTH) {
		const auto cutOffIndex = existingLogData.indexOf(existingLogDataLength - MAX_LOG_LENGTH, "\n");
		if (cutOffIndex == -1) {
			existingLogData = "[...]\n" + existingLogData.substring(existingLogDataLength - MAX_LOG_LENGTH);
		}
		else {
			existingLogData = "[...]\n" + existingLogData.substring(cutOffIndex + 1);
		}
	}

	const juce::String newLogData = existingLogData + "\n" + makeIndentation(indent) + text;
	logTree.setProperty(PROPS::LogData, newLogData, nullptr);
}

/*void logMidiMessage(const juce::String& txt)
{
	//String tmp = statusLog.getValue().toString();
	//tmp.append("\n" + txt, 2000);
	logData += "\n" + txt;
}*/

void StatusLog::log(const juce::String& txt, int indent)
{
	// DBG(txt);
	addToLog(txt, indent);
}

void StatusLog::debug(const juce::String& txt, int indent)
{
#ifdef _DEBUG
	DBG(txt);
	addToLog(txt, indent);
#endif
}



void StatusLog::logMidiMessage(const juce::MidiMessage& message, const juce::String& source)
{
	const double time = message.getTimeStamp();

	// useless as contained time info seems relative to something, beat or such. Output is not helpful.
	//const int hours = ((int)(time / 3600.0)) % 24;
	//const int minutes = ((int)(time / 60.0)) % 60;
	//const int seconds = ((int)time) % 60;
	//const int millis = ((int)(time * 1000.0)) % 1000;
	//const juce::String timecode(juce::String::formatted("%02d:%02d:%02d.%03d", hours, minutes, seconds, millis));

	const juce::String description(getMidiMessageDescription(message));
	const juce::String midiMessageString(description + " (" + source + ")");

	log(midiMessageString);
}

juce::String StatusLog::getMidiMessageDescription(const juce::MidiMessage& m)
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


