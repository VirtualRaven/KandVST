

#include "PluginProcessor.h"
#include "PluginGui.h"
//==============================================================================
PluginGUI::PluginGUI(PluginProcessor& owner)
	: AudioProcessorEditor(owner),
	ourLookAndFeel(),
	__tabComponent(TabbedButtonBar::Orientation::TabsAtTop),
	__keyboard(owner.keyboardState,MidiKeyboardComponent::Orientation::horizontalKeyboard),
	__cc()
{
	setResizable(false, false);
	setSize(1000, 720);
	__loadingImage.setImage(ImageFileFormat::loadFrom(Resources::Images::loading_png,sizeof(Resources::Images::loading_png)));

	addAndMakeVisible(__loadingImage);
	__loadingImage.setBounds(getLocalBounds());

}


void PluginGUI::InitializeGui()
{
	removeAllChildren();
	setLookAndFeel(&ourLookAndFeel);

	__tabComponent.addTab("M", Colours::darkgreen, new MasterComponent(), true);

	for (int i = 0; i < 4; i++)
	{
		__tabComponent.addTab(std::to_string(i + 1), Colours::darkgrey, new OscillatorPageComponent(i), true);
	}

	__tabComponent.addTab("Console", Colours::darkgrey, &__cc, true);

	addAndMakeVisible(__tabComponent);
	addAndMakeVisible(__keyboard);
	__keyboard.setKeyWidth(__keyboard.getKeyWidth() + 10.0f);


}



PluginGUI::~PluginGUI()
{
	//Has to unset our lookAndFeel before it is destroyed
	this->setLookAndFeel(nullptr);
}

//==============================================================================
void PluginGUI::paint (Graphics& g)
{
    g.setColour (Colour::fromRGB(36,36,36));
    g.fillAll();

	Image titleImage = ImageFileFormat::loadFrom(Resources::Images::Title3_png, sizeof(Resources::Images::Title3_png));
	g.setOpacity(1.0f);
	g.drawImageAt(titleImage, 1000, 0, false);
	updateTimecodeDisplay(getProcessor().lastPosInfo);
}

void PluginGUI::resized()
{
	Rectangle<int> localBounds (getLocalBounds().reduced(8));

	__keyboard.setBounds(localBounds.removeFromBottom(100).removeFromRight(getLocalBounds().getWidth() - 150).removeFromLeft(getLocalBounds().getWidth() - 150));
	__keyboard.setCentrePosition(localBounds.getCentreX(), __keyboard.getBounds().getBottom()-(__keyboard.getHeight()/2));	
	localBounds.removeFromBottom(8); //Padding
	__tabComponent.setBounds(localBounds);

}

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

void PluginGUI::updateTimecodeDisplay(AudioPlayHead::CurrentPositionInfo pos)
{
	if (!pos.isPlaying) return;
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

	Global->log->Write(displayText.toString());
	//timecodeDisplayLabel.setText(displayText.toString(), dontSendNotification);
}
