#include "FilterComponent.h"

FilterComponent::~FilterComponent()
{

}
FilterComponent::FilterComponent(int ID)
{

	__hpFilter = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(ID, "FILTER_HP"));
	__lpFilter = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(ID, "FILTER_LP"));

	addAndMakeVisible(__hpFilter);
	addAndMakeVisible(__lpFilter);

	__lpFilter->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__lpFilter->setTextBoxStyle(__lpFilter->NoTextBox, true, 10, 10);
	__lpFilter->setSkewFactorFromMidPoint(0.15);
	__lpLabel.setText("LP", NotificationType::dontSendNotification);
	__lpLabel.attachToComponent(__lpFilter, false);
	__lpLabel.setJustificationType(juce::Justification::horizontallyCentred);

	__hpFilter->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__hpFilter->setTextBoxStyle(__hpFilter->NoTextBox, true, 10, 10);
	__hpFilter->setSkewFactorFromMidPoint(0.15);
	__hpLabel.setText("HP", NotificationType::dontSendNotification);
	__hpLabel.attachToComponent(__hpFilter, false);
	__hpLabel.setJustificationType(juce::Justification::horizontallyCentred);

}

void FilterComponent::paint(Graphics& g) {

	int width = __bounds.getWidth();
	int height = __bounds.getHeight();
	int fontHeight = __bounds.getHeight() * 0.2;
	int fontSize = __bounds.getHeight() * 0.16;

	g.setColour(Colour::fromRGB(60, 60, 60));
	g.drawRect(Rectangle<float>(10, 10, width, height), 2.0f);
	g.fillRect(Rectangle<float>(10, 10, width, fontHeight));
	
	g.setColour(Colours::white);
	Font filter(fontSize, Font::FontStyleFlags::bold);
	g.setFont(filter);
	g.drawText("FILTER", Rectangle<int>(10, 10, width, fontHeight), Justification::centred, false);
	
}

void FilterComponent::resized() {

	__bounds = getLocalBounds().reduced(5);
	__lpLabel.setFont(Font(__bounds.getHeight() * 0.13, Font::plain));
	__hpLabel.setFont(Font(__bounds.getHeight() * 0.13, Font::plain));

	Rectangle<int> __filterBounds(__bounds);

	int __size = (__bounds.getHeight() * 0.55);
	
	__filterBounds.removeFromRight(__bounds.getWidth() * 0.05);
	
	__lpFilter->setBounds(__filterBounds.removeFromRight(__size).removeFromBottom(__size));

	__filterBounds.removeFromRight(__bounds.getWidth() * 0.1);
	
	__hpFilter->setBounds(__filterBounds.removeFromRight(__size).removeFromBottom(__size));


}