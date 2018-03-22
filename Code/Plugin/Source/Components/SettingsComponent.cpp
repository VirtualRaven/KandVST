#include "SettingsComponent.h"

SettingsComponent::~SettingsComponent()
{

}

SettingsComponent::SettingsComponent()
{

	__pitchBendSens = new ParameterSlider(*Global->paramHandler->Get<AudioParameterInt>(-1, "PITCH_BEND_SENS"));
	__selectKnob = new SelectKnob(*Global->paramHandler->Get<AudioParameterChoice>(-1, "SELECT_KNOB_VALUE"));

	addAndMakeVisible(__pitchBendSens);
	addAndMakeVisible(__selectKnob);

	__pitchBendSens->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__pitchBendSens->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 50, 15);
	__pitchBendLabel.setText("PB Sens", NotificationType::dontSendNotification);
	__pitchBendLabel.attachToComponent(__pitchBendSens, false);
	__pitchBendLabel.setJustificationType(juce::Justification::centred);

}

void SettingsComponent::paint(Graphics& g) {

	int width = __bounds.getWidth();
	int height = __bounds.getHeight();
	int fontHeight = __bounds.getHeight() * 0.1;
	int fontSize = __bounds.getHeight() * 0.06;

	g.setColour(Colour::fromRGB(60, 60, 60));
	g.drawRect(Rectangle<int>(0, 0, width, height), 2.0f);
	g.fillRect(Rectangle<int>(0, 0, width, fontHeight));

	g.setColour(Colours::white);
	Font filter(fontSize, Font::FontStyleFlags::bold);
	g.setFont(filter);
	g.drawText("SETTINGS", Rectangle<int>(0, 0, width, fontHeight), Justification::centred, false);
	
}

void SettingsComponent::resized() 
{
	
	__bounds = getLocalBounds();
	int fontHeight = __bounds.getHeight() * 0.1;
	Rectangle<int> settings(__bounds.reduced(8));
	settings.removeFromTop(fontHeight);
	settings.removeFromTop(__pitchBendLabel.getFont().getHeight());
	int space = settings.getHeight() * 0.4;
	settings.removeFromBottom(settings.getHeight() * 0.1);
	__pitchBendSens->setBounds(settings.removeFromBottom(space));
	settings.removeFromBottom(settings.getHeight() * 0.1);
	__selectKnob->setBounds(settings);

}

void SettingsComponent::RegisterParameters(int ID)
{
	Global->paramHandler->RegisterInt(ID, "PITCH_BEND_SENS", "Pitch bend sensitivity", 1, 12, 2);
        auto tmp = StringArray("1", "2", "3", "4", "5");
	Global->paramHandler->RegisterChoice(ID, "SELECT_KNOB_VALUE", "Select Knob",tmp , 1);
}
