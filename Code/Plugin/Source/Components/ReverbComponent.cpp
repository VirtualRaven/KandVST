#include "ReverbComponent.h"

ReverbComponent::ReverbComponent(int ID, GLOBAL *global) :
IVSTParameters(-1)
{
	Global = global;

	addAndMakeVisible(__toggleReverb = new ParameterButton(*Global->paramHandler->Get<AudioParameterBool>(__ID, "REVERB_EN")));
	__toggleReverb->setButtonText("REVERB");

	addAndMakeVisible(__dry = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "REVERB_DRY"), Global));
	__dry->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__dry->setTextBoxStyle(__dry->NoTextBox, true, 50, 15);
	__dryLabel.setText("DRY", NotificationType::dontSendNotification);
	__dryLabel.attachToComponent(__dry, false);
	__dryLabel.setJustificationType(Justification::centred);

	addAndMakeVisible(__wet = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "REVERB_WET"), Global));
	__wet->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__wet->setTextBoxStyle(__wet->NoTextBox, true, 50, 15);
	__wetLabel.setText("WET", NotificationType::dontSendNotification);
	__wetLabel.attachToComponent(__wet, false);
	__wetLabel.setJustificationType(Justification::centred);
	
	addAndMakeVisible(__cBox);
	__cBox.addListener(this);
	__cBox.setColour(ComboBox::backgroundColourId, Swatch::background);
	__cBox.setColour(ComboBox::buttonColourId, Swatch::white);
	__ir = StringArray("Nuclear reactor", "Cathedral", "Living room 1", "Living room 2", "Empty room", "Bathtub");
	__cBox.addItemList(__ir, 1);
	__cBox.setSelectedId(0, false);
	
}

ReverbComponent::~ReverbComponent() {

}

void ReverbComponent::comboBoxChanged(ComboBox * cbox) {
	*Global->paramHandler->Get<AudioParameterChoice>(__ID, "REVERB_IR") = cbox->getSelectedId();
}

void ReverbComponent::paint(Graphics & g)
{
	g.setColour(Swatch::outlineColour);

	int width = getLocalBounds().getWidth();
	int height = getLocalBounds().getHeight();
	int fontHeight = getLocalBounds().getHeight() * 0.2;
	g.drawRect(Rectangle<int>(0, 0, width, height), 2.0f);
	g.fillRect(Rectangle<int>(0, 0, width, fontHeight));
	
	g.setColour(Colours::white);

	if (!*Global->paramHandler->Get<AudioParameterBool>(__ID, "REVERB_EN")) 
	{ //if disabled
		__dry->setEnabled(false);
		__wet->setEnabled(false);
		__cBox.setEnabled(false);

		__dryLabel.setColour(Label::textColourId, Swatch::disabled);
		__wetLabel.setColour(Label::textColourId, Swatch::disabled);
		__toggleReverb->setColour(ParameterButton::textColourOffId, Swatch::disabled);

	}
	else {
		__dry->setEnabled(true);
		__wet->setEnabled(true);
		__cBox.setEnabled(true);

		__dryLabel.setColour(Label::textColourId, Swatch::white);
		__wetLabel.setColour(Label::textColourId, Swatch::white);
		__toggleReverb->setColour(ParameterButton::textColourOffId, Swatch::white);
	}
}

void ReverbComponent::resized()
{
	Rectangle<int> bounds(getLocalBounds());
	__toggleReverb->setBounds(bounds.removeFromTop(bounds.getHeight() * 0.2));
	__cBox.setBounds(bounds.removeFromTop(bounds.getHeight() * 0.4).reduced(8));
	bounds.removeFromTop(15);
	int size = bounds.getWidth() * 0.5f;
	__dry->setBounds(bounds.removeFromLeft(size));
	__wet->setBounds(bounds);

	
}
