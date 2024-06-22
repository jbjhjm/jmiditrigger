# jMidiTrigger2

jMidiTrigger is a midi-processing VST plugin able to map Midi input, likely Notes sent from a DAW midi track, to arbitrary Midi Output data defined in a XML file.

## development

jMidiTrigger2 is implemented using juce framework 7.0.5 and has valid projects for visual studio CE 2022 (C++ 145)

## Helpful resources

* VST 3 build/debug configuration https://forum.juce.com/t/windows-where-is-the-vst3-build/36600/10

## debugging

Place a copy of SAVIhost 3/x64 in .\jmiditrigger\Builds\VisualStudio2022\x64\Debug\VST3,
name it same as generated plugin file (currently NewProject.vst) and set it as debug executable in VST3 project settings -> debugging.
