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

// variables
int startY = 150;
Label title;
TabbedButtonBar buttonBar(TabbedButtonBar::Orientation::TabsAtTop);

//==============================================================================
JuceDemoPluginAudioProcessorEditor::JuceDemoPluginAudioProcessorEditor (JuceDemoPluginAudioProcessor& owner)
    : AudioProcessorEditor (owner),
	  envelopeComponent(0),
      midiKeyboard (owner.keyboardState, MidiKeyboardComponent::horizontalKeyboard),
      delayLabel (String(), "Delay:"),
	  cc(),
	  waveType(),
      infoWindow()
{
    
	//addAndMakeVisible(cc);

    /*
    // add the title 
    addAndMakeVisible(title);
    title.setFont(Font("Quantum",80, Font::plain));
    title.setText("KANDVST", NotificationType::dontSendNotification);
    title.setJustificationType(Justification::centred);
*/

    // add button bar
    addAndMakeVisible(buttonBar);
    buttonBar.addTab("M", Colours::darkgreen, 0);
    for(int i = 1; i < 4; i++){
        buttonBar.addTab(std::to_string(i), Colours::darkgrey, i);
    }
    

/*
    // add the wavetable
	addAndMakeVisible(waveType);
	waveType.addItem("Sine", 1);
	waveType.addItem("Square", 2);
	waveType.addItem("Saw", 3);
	waveType.addItem("Triangle", 4);
	waveType.setSelectedId(1);
	waveType.addListener(this);
*/

    // add envelope components
	addAndMakeVisible(envelopeComponent);
   
    // add some sliders..
    addAndMakeVisible (delaySlider = new ParameterSlider (*Global.paramHandler->Get<AudioParameterFloat>(0,"ENV_ATTACK")));
    delaySlider->setSliderStyle (Slider::Rotary);

    // add the info component..
    addAndMakeVisible(infoWindow);

    delayLabel.attachToComponent (delaySlider, false);
    delayLabel.setFont (Font (11.0f));

    // add the midi keyboard component..
    addAndMakeVisible (midiKeyboard);

    // fixed plugin size
    setResizable(false,false);

    // set our component's initial size to be the last one that was stored in the filter's settings
    setSize (1280,720);

    //========================================================
    updateTrackProperties();

    // start a timer which will keep our timecode display updated
    startTimerHz (30);
}

JuceDemoPluginAudioProcessorEditor::~JuceDemoPluginAudioProcessorEditor()
{
}

//==============================================================================
void JuceDemoPluginAudioProcessorEditor::paint (Graphics& g)
{
    g.setColour (Colour::fromRGB(36,36,36));
    g.fillAll();
}

void JuceDemoPluginAudioProcessorEditor::resized()
{

    // This lays out our child components...
    Rectangle<int> r (getLocalBounds().reduced (8));
    Rectangle<int> lb(getLocalBounds());
    
    /*
    // TITLE
    title.setBounds(getLocalBounds());
    title.setCentrePosition(lb.getCentre().getX(), 50);
    */

    // buttonBar
    Rectangle<int> buttonBounds(getLocalBounds());
    buttonBar.setBounds(r.removeFromTop(50));

    infoWindow.setBounds(getLocalBounds());
    infoWindow.setCentrePosition(getLocalBounds().getCentre());

    midiKeyboard.setBounds (r.removeFromBottom(70).removeFromRight(getLocalBounds().getWidth()-100).removeFromLeft(getLocalBounds().getWidth()-100));
    r.removeFromTop (20);
	//envelopeComponent.setBounds(r.removeFromTop(100).removeFromRight(200));
    
    envelopeComponent.setBounds(getLocalBounds().getWidth()-200, startY, getLocalBounds().getWidth(), startY);
	//waveType.setBounds(r.removeFromTop(100));
	//Rectangle<int> sliderArea2(r.removeFromTop(130));
	/*delaySlider->setBounds(sliderArea2.removeFromLeft(jmin(180, sliderArea2.getWidth())));*/
	//cc.setBounds(r.removeFromTop(300));
	
    getProcessor().lastUIWidth = getWidth();
    getProcessor().lastUIHeight = getHeight();
}

//==============================================================================
void JuceDemoPluginAudioProcessorEditor::timerCallback()
{
	AudioParameterInt* wt = Global.paramHandler->Get<AudioParameterInt>(0, "WAVE_TYPE");
	waveType.setSelectedId(*wt+1);
    updateTimecodeDisplay (getProcessor().lastPosInfo);
}
void JuceDemoPluginAudioProcessorEditor::hostMIDIControllerIsAvailable (bool controllerIsAvailable)
{
    midiKeyboard.setVisible (! controllerIsAvailable);
}

void JuceDemoPluginAudioProcessorEditor::updateTrackProperties ()
{
    auto trackColour = getProcessor().trackProperties.colour;
    auto& lf = getLookAndFeel();

    backgroundColour = (trackColour == Colour() ? lf.findColour (ResizableWindow::backgroundColourId)
                                                : trackColour.withAlpha (1.0f).withBrightness (0.266f));
    repaint();
}

//==============================================================================
// quick-and-dirty function to format a timecode string
static String timeToTimecodeString (double seconds)
{
    const int millisecs = roundToInt (seconds * 1000.0);
    const int absMillisecs = std::abs (millisecs);

    return String::formatted ("%02d:%02d:%02d.%03d",
                              millisecs / 3600000,
                              (absMillisecs / 60000) % 60,
                              (absMillisecs / 1000) % 60,
                              absMillisecs % 1000);
}

// quick-and-dirty function to format a bars/beats string
static String quarterNotePositionToBarsBeatsString (double quarterNotes, int numerator, int denominator)
{
    if (numerator == 0 || denominator == 0)
        return "1|1|000";

    const int quarterNotesPerBar = (numerator * 4 / denominator);
    const double beats  = (fmod (quarterNotes, quarterNotesPerBar) / quarterNotesPerBar) * numerator;

    const int bar    = ((int) quarterNotes) / quarterNotesPerBar + 1;
    const int beat   = ((int) beats) + 1;
    const int ticks  = ((int) (fmod (beats, 1.0) * 960.0 + 0.5));

    return String::formatted ("%d|%d|%03d", bar, beat, ticks);
}

// Updates the text in our position label.
void JuceDemoPluginAudioProcessorEditor::updateTimecodeDisplay (AudioPlayHead::CurrentPositionInfo pos)
{
    MemoryOutputStream displayText;

    displayText << "[" << SystemStats::getJUCEVersion() << "]   "
                << String (pos.bpm, 2) << " bpm, "
                << pos.timeSigNumerator << '/' << pos.timeSigDenominator
                << "  -  " << timeToTimecodeString (pos.timeInSeconds)
                << "  -  " << quarterNotePositionToBarsBeatsString (pos.ppqPosition,
                                                                    pos.timeSigNumerator,
                                                                    pos.timeSigDenominator);

    if (pos.isRecording)
        displayText << "  (recording)";
    else if (pos.isPlaying)
        displayText << "  (playing)";

    timecodeDisplayLabel.setText (displayText.toString(), dontSendNotification);
}

void JuceDemoPluginAudioProcessorEditor::comboBoxChanged(ComboBox * comboBoxThatHasChanged)
{
	//Race with timer
	AudioParameterInt* wt = Global.paramHandler->Get<AudioParameterInt>(0, "WAVE_TYPE");
	if (waveType.getSelectedId() != 0)
		*wt = waveType.getSelectedId() - 1;
}
