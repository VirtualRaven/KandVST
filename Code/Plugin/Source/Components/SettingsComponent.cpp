#include "SettingsComponent.h"

SettingsComponent::~SettingsComponent()
{

}

SettingsComponent::SettingsComponent()
{

	__pitchBendSens = new ParameterSlider(*Global->paramHandler->Get<AudioParameterInt>(-1, "PITCH_BEND_SENS"));

	addAndMakeVisible(__pitchBendSens);

	__pitchBendSens->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__pitchBendSens->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 50, 15);
	__pitchBendLabel.setText("PB Sens", NotificationType::dontSendNotification);
	__pitchBendLabel.attachToComponent(__pitchBendSens, false);
	__pitchBendLabel.setJustificationType(juce::Justification::centred);

}

void SettingsComponent::paint(Graphics& g) {
	g.setColour(Colour::fromRGB(60, 60, 60));
	g.drawRect(Rectangle<int>(10, 10, 220, 130), 2.0f);
	g.fillRect(Rectangle<int>(10, 10, 220, 30));

	g.setColour(Colours::white);
	Font filter(20, Font::FontStyleFlags::bold);
	g.setFont(filter);
	g.drawText("SETTINGS", Rectangle<int>(10, 10, 220, 30), Justification::centred, false);
	
}

void SettingsComponent::resized() 
{
	int size = 50;
	int gap = 25;

	Rectangle<int> sliders(getLocalBounds().reduced(8));
	sliders.removeFromTop(60);
	sliders.removeFromLeft(gap);
	__pitchBendSens->setBounds(sliders.removeFromLeft(size).removeFromTop(size + 15));
}

void SettingsComponent::RegisterParameters(int ID)
{
	Global->paramHandler->RegisterInt(ID, "PITCH_BEND_SENS", "Pitch bend sensitivity", 1, 12, 2);

}