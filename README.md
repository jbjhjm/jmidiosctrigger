# JMidiOscTrigger

JMidiOscTrigger is a midi-processing VST that sends OSC commands according to definitions read from a XML file.

## Installation

Install VC++ 2022 redistributable.

## development

JMidiOscTrigger is implemented using juce framework 7.0.5 and has valid projects for visual studio CE 2022 (C++ 145)

## Helpful resources

* VST 3 build/debug configuration https://forum.juce.com/t/windows-where-is-the-vst3-build/36600/10

## debugging

Place a copy of SAVIhost 3/x64 in .\jmidiosctrigger\Builds\VisualStudio2022\x64\Debug\VST3,
name it same as generated plugin file (currently NewProject.vst) and set it as debug executable in VST3 project settings -> debugging.

Note for myself:
VS2022 target "DebugReaper" is configurated to copy debug build to local portable Reaper and launch it.
