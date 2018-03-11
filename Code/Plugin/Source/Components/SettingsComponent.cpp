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

	int width = __bounds.getWidth();
	int height = __bounds.getHeight();
	int fontHeight = __bounds.getHeight() * 0.2;
	int fontSize = __bounds.getHeight() * 0.14;

	g.setColour(Colour::fromRGB(60, 60, 60));
	g.drawRect(Rectangle<int>(10, 10, width, height), 2.0f);
	g.fillRect(Rectangle<int>(10, 10, width, fontHeight));

	g.setColour(Colours::white);
	Font filter(fontSize, Font::FontStyleFlags::bold);
	g.setFont(filter);
	g.drawText("SETTINGS", Rectangle<int>(10, 10, width, fontHeight), Justification::centred, false);
	
}

void SettingsComponent::resized() 
{
	__bounds = getLocalBounds().reduced(5);

	int size = __bounds.getHeight() * 0.4;
	int gap = size * 0.5;

	Rectangle<int> sliders(__bounds);
	sliders.removeFromTop( __bounds.getHeight() * 0.45);
	sliders.removeFromLeft(gap);
	__pitchBendSens->setBounds(sliders.removeFromLeft(size).removeFromTop(size + 15));
}

void SettingsComponent::RegisterParameters(int ID)
{
	Global->paramHandler->RegisterInt(ID, "PITCH_BEND_SENS", "Pitch bend sensitivity", 1, 12, 2);

}