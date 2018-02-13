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

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JuceDemoPluginAudioProcessorEditor::JuceDemoPluginAudioProcessorEditor(JuceDemoPluginAudioProcessor& owner)
	: AudioProcessorEditor(owner),
	envelopeComponent(0),
	midiKeyboard(owner.keyboardState, MidiKeyboardComponent::horizontalKeyboard),
	timecodeDisplayLabel(String()),
	delayLabel(String(), "Delay:"),
	octaveLabel(String(), "Octave:"),
	offsetLabel(String(), "Offset:"),
	detuneLabel(String(), "Detune:"),
	cc(),
	waveType(), oscOctave()
{
	// add some sliders..

	addAndMakeVisible(cc);

	addAndMakeVisible(waveType);
	waveType.addItem("Sine", 1);
	waveType.addItem("Square", 2);
	waveType.addItem("Saw", 3);
	waveType.addItem("Triangle", 4);
	waveType.setSelectedId(1);
	waveType.addListener(this);
	addAndMakeVisible(envelopeComponent);



	addAndMakeVisible(octaveSlider = new ParameterSlider(*Global.paramHandler->Get<AudioParameterInt>(0, "OSC_OCTAVE")));
	octaveSlider->setSliderStyle(Slider::Rotary);
	octaveLabel.attachToComponent(octaveSlider, false);
	addAndMakeVisible(offsetSlider = new ParameterSlider(*Global.paramHandler->Get<AudioParameterInt>(0, "OSC_OFFSET")));
	offsetSlider->setSliderStyle(Slider::Rotary);
	offsetLabel.attachToComponent(offsetSlider, false);
	addAndMakeVisible(detuneSlider = new ParameterSlider(*Global.paramHandler->Get<AudioParameterFloat>(0, "OSC_DETUNE")));
	detuneSlider->setSliderStyle(Slider::Rotary);
	detuneLabel.attachToComponent(detuneSlider, false);



	delayLabel.attachToComponent(delaySlider, false);
	delayLabel.setFont(Font(11.0f));

	// add the midi keyboard component..
	addAndMakeVisible(midiKeyboard);

	// add a label that will display the current timecode and status..
	addAndMakeVisible(timecodeDisplayLabel);
	timecodeDisplayLabel.setFont(Font(Font::getDefaultMonospacedFontName(), 15.0f, Font::plain));

	// set resize limits for this plug-in
	setResizeLimits(180 * 4, 330, 1024, 700);

	// set our component's initial size to be the last one that was stored in the filter's settings
	setSize(owner.lastUIWidth,
		owner.lastUIHeight);

	updateTrackProperties();

	// start a timer which will keep our timecode display updated
	startTimerHz(30);
}

JuceDemoPluginAudioProcessorEditor::~JuceDemoPluginAudioProcessorEditor()
{
}

//==============================================================================
void JuceDemoPluginAudioProcessorEditor::paint(Graphics& g)
{
	g.setColour(backgroundColour);
	g.fillAll();
}

void JuceDemoPluginAudioProcessorEditor::resized()
{
	// This lays out our child components...

	Rectangle<int> r(getLocalBounds().reduced(8));

	timecodeDisplayLabel.setBounds(r.removeFromTop(26));
	midiKeyboard.setBounds(r.removeFromBottom(70));

	r.removeFromTop(20);

	envelopeComponent.setBounds(r.removeFromTop(100).removeFromRight(200));
	Rectangle<int> sliderArea2(r.removeFromTop(50));
	octaveSlider->setBounds(sliderArea2.removeFromLeft(jmin(150, sliderArea2.getWidth())));
	offsetSlider->setBounds(sliderArea2.removeFromLeft(jmin(150, sliderArea2.getWidth())));
	detuneSlider->setBounds(sliderArea2.removeFromLeft(jmin(150, sliderArea2.getWidth())));
	waveType.setBounds(r.removeFromTop(100));
	cc.setBounds(r.removeFromTop(300));

	getProcessor().lastUIWidth = getWidth();
	getProcessor().lastUIHeight = getHeight();
}

//==============================================================================
void JuceDemoPluginAudioProcessorEditor::timerCallback()
{
	AudioParameterInt* wt = Global.paramHandler->Get<AudioParameterInt>(0, "WAVE_TYPE");
	waveType.setSelectedId(*wt + 1);
	updateTimecodeDisplay(getProcessor().lastPosInfo);
}
void JuceDemoPluginAudioProcessorEditor::hostMIDIControllerIsAvailable(bool controllerIsAvailable)
{
	midiKeyboard.setVisible(!controllerIsAvailable);
}

void JuceDemoPluginAudioProcessorEditor::updateTrackProperties()
{
	auto trackColour = getProcessor().trackProperties.colour;
	auto& lf = getLookAndFeel();

	backgroundColour = (trackColour == Colour() ? lf.findColour(ResizableWindow::backgroundColourId)
		: trackColour.withAlpha(1.0f).withBrightness(0.266f));
	repaint();
}

//==============================================================================
// quick-and-dirty function to format a timecode string
static String timeToTimecodeString(double seconds)
{
	const int millisecs = roundToInt(seconds * 1000.0);
	const int absMillisecs = std::abs(millisecs);

	return String::formatted("%02d:%02d:%02d.%03d",
		millisecs / 3600000,
		(absMillisecs / 60000) % 60,
		(absMillisecs / 1000) % 60,
		absMillisecs % 1000);
}

// quick-and-dirty function to format a bars/beats string
static String quarterNotePositionToBarsBeatsString(double quarterNotes, int numerator, int denominator)
{
	if (numerator == 0 || denominator == 0)
		return "1|1|000";

	const int quarterNotesPerBar = (numerator * 4 / denominator);
	const double beats = (fmod(quarterNotes, quarterNotesPerBar) / quarterNotesPerBar) * numerator;

	const int bar = ((int)quarterNotes) / quarterNotesPerBar + 1;
	const int beat = ((int)beats) + 1;
	const int ticks = ((int)(fmod(beats, 1.0) * 960.0 + 0.5));

	return String::formatted("%d|%d|%03d", bar, beat, ticks);
}

// Updates the text in our position label.
void JuceDemoPluginAudioProcessorEditor::updateTimecodeDisplay(AudioPlayHead::CurrentPositionInfo pos)
{
	MemoryOutputStream displayText;

	displayText << "[" << SystemStats::getJUCEVersion() << "]   "
		<< String(pos.bpm, 2) << " bpm, "
		<< pos.timeSigNumerator << '/' << pos.timeSigDenominator
		<< "  -  " << timeToTimecodeString(pos.timeInSeconds)
		<< "  -  " << quarterNotePositionToBarsBeatsString(pos.ppqPosition,
			pos.timeSigNumerator,
			pos.timeSigDenominator);

	if (pos.isRecording)
		displayText << "  (recording)";
	else if (pos.isPlaying)
		displayText << "  (playing)";

	timecodeDisplayLabel.setText(displayText.toString(), dontSendNotification);
}

void JuceDemoPluginAudioProcessorEditor::comboBoxChanged(ComboBox * comboBoxThatHasChanged)
{
	//Race with timer
	AudioParameterInt* wt = Global.paramHandler->Get<AudioParameterInt>(0, "WAVE_TYPE");
	if (waveType.getSelectedId() != 0)
		*wt = waveType.getSelectedId() - 1;
}