/*
==============================================================================

This file is part of the JUCE library.
Copyright (c) 2017 - ROLI Ltd.

JUCE is an open source library subject to commercial or open-source
licensing.

By using JUCE, you agree to the terms of both the JUCE 5 End-User License
Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
27th April 2017).

End User License Agreement: www.juce.com/juce-5-licence
Privacy Policy: www.juce.com/juce-5-privacy-policy

Or: You may also use this code under the terms of the GPL v3 (see
www.gnu.org/licenses).

JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
DISCLAIMED.

==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Global.h"
#include "ParameterHandler.h"
#include "Components\ConsoleComponent.h"
#include "Components\ParameterSlider.h"
#include "Components\EnvelopeComponent.h"

//==============================================================================
/** This is the editor component that our filter will display.
*/
class JuceDemoPluginAudioProcessorEditor : public AudioProcessorEditor,
	private Timer,
	private ComboBox::Listener

{
public:
	JuceDemoPluginAudioProcessorEditor(PluginProcessor&);
	~JuceDemoPluginAudioProcessorEditor();

	//==============================================================================
	void paint(Graphics&) override;
	void resized() override;
	void timerCallback() override;
	void hostMIDIControllerIsAvailable(bool) override;
	void updateTrackProperties();

private:
	EnvelopeComponent envelopeComponent;
	MidiKeyboardComponent midiKeyboard;
	Label timecodeDisplayLabel, delayLabel, octaveLabel, detuneLabel, offsetLabel;
	ScopedPointer<ParameterSlider>delaySlider, octaveSlider, detuneSlider, offsetSlider;
	Colour backgroundColour;
	ComboBox waveType, oscOctave;
	ConsoleComponent cc;

	//==============================================================================
	PluginProcessor& getProcessor() const
	{
		return static_cast<PluginProcessor&> (processor);
	}

	void updateTimecodeDisplay(AudioPlayHead::CurrentPositionInfo);

	// Inherited via Listener
	virtual void comboBoxChanged(ComboBox * comboBoxThatHasChanged) override;
};