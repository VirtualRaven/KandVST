#include "SettingsComponent.h"

SettingsComponent::~SettingsComponent()
{

}

SettingsComponent::SettingsComponent(GLOBAL*global)
{
	Global = global;
	__pitchBendSens = new ParameterSlider(*Global->paramHandler->Get<AudioParameterInt>(-1, "PITCH_BEND_SENS"),global);
	__selectKnob = new ParameterSlider(*Global->paramHandler->Get<AudioParameterChoice>(-1, "SELECT_KNOB_VALUE"),global);
	__selectKnob->setSize(50, 50);

	addAndMakeVisible(__pitchBendSens);
	addAndMakeVisible(__selectKnob);

	__pitchBendSens->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__pitchBendSens->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 50, 15);
	__pitchBendLabel.setText("PB Sens", NotificationType::dontSendNotification);
	__pitchBendLabel.attachToComponent(__pitchBendSens, false);
	__pitchBendLabel.setJustificationType(juce::Justification::centred);

}

void SettingsComponent::paint(Graphics& g)
{
	int width = __bounds.getWidth();
	int height = __bounds.getHeight();
	int fontHeight = __bounds.getHeight() * 0.2;
	int fontSize = __bounds.getHeight() * 0.16;

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
	int fontHeight = __bounds.getHeight() * 0.2;
	Rectangle<int> settings(__bounds.reduced(8));
	settings.removeFromTop(fontHeight);
	settings.removeFromTop(__pitchBendLabel.getFont().getHeight());
	
	int space = settings.getWidth() * 0.5;
	
	__pitchBendSens->setBounds(settings.removeFromLeft(space));
	
	
	__selectKnob->setBounds(settings.removeFromLeft(2*space));

}

void SettingsComponent::RegisterParameters(int ID,GLOBAL*Global)
{
	Global->paramHandler->RegisterInt(ID, "PITCH_BEND_SENS", "Pitch bend sensitivity", 1, 12, 2);
        auto tmp = StringArray("1", "2", "3", "4", "5");
	Global->paramHandler->RegisterChoice(ID, "SELECT_KNOB_VALUE", "Select Knob",tmp , 1);
}
