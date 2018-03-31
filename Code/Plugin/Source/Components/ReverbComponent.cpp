#include "ReverbComponent.h"

ReverbComponent::ReverbComponent(int ID, GLOBAL* global) :
__ID(ID)
{
	Global = global;

	/*
	addAndMakeVisible(__dry = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "REVERB_DRY"), Global));
	__dry->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__dry->setTextBoxStyle(__dry->NoTextBox, true, 50, 15);
	__dry->setName("DRY");

	addAndMakeVisible(__wet = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "REVERB_WET"), Global));
	__wet->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__wet->setTextBoxStyle(__wet->NoTextBox, true, 50, 15);
	__wet->setName("WET");

	*/
	addAndMakeVisible(__cBox);
	__cBox.setName("Impulse Responses");
	__cBox.addListener(this);
}

ReverbComponent::~ReverbComponent() {

}

void ReverbComponent::comboBoxChanged(ComboBox * cbox) {
	int current = cbox->getSelectedId();
}

void ReverbComponent::paint(Graphics & g)
{
	int width = getLocalBounds().getWidth();
	int height = getLocalBounds().getHeight();
	int fontHeight = getLocalBounds().getHeight() * 0.1;
	int fontSize = getLocalBounds().getHeight() * 0.06;

	g.setColour(Colour::fromRGB(60, 60, 60));
	g.drawRect(Rectangle<int>(0, 0, width, height), 2.0f);
	g.fillRect(Rectangle<int>(0, 0, width, fontHeight));

	g.setColour(Colours::white);
	Font filter(fontSize, Font::FontStyleFlags::bold);
	g.setFont(filter);
	g.drawText("REVERB", Rectangle<int>(0, 0, width, fontHeight), Justification::centred, false);
}

void ReverbComponent::resized()
{
	
}
