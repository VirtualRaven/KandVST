#include "FilterComponent.h"

FilterComponent::~FilterComponent()
{

}

FilterComponent::FilterComponent()
{

	__hpFilter = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(-1, "FILTER_HP"));
	__lpFilter = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(-1, "FILTER_LP"));

	addAndMakeVisible(__hpFilter);
	addAndMakeVisible(__lpFilter);

	__lpFilter->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__lpFilter->setTextBoxStyle(__lpFilter->NoTextBox, true, 10, 10);
	__lpFilter->setSkewFactorFromMidPoint(0.15);
	__lpLabel.setText("LP", NotificationType::dontSendNotification);
	__lpLabel.attachToComponent(__lpFilter, false);
	__lpLabel.setJustificationType(juce::Justification::centred);

	__hpFilter->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__hpFilter->setTextBoxStyle(__hpFilter->NoTextBox, true, 10, 10);
	__hpFilter->setSkewFactorFromMidPoint(0.15);
	__hpLabel.setText("HP", NotificationType::dontSendNotification);
	__hpLabel.attachToComponent(__hpFilter, false);
	__hpLabel.setJustificationType(juce::Justification::centred);

}

void FilterComponent::paint(Graphics& g) {
	g.setColour(Colour::fromRGB(60, 60, 60));
	g.drawRect(Rectangle<int>(10, 10, 220, 130), 2.0f);
	g.fillRect(Rectangle<int>(10, 10, 220, 30));

	g.setColour(Colours::white);
	Font filter(20, Font::FontStyleFlags::bold);
	g.setFont(filter);
	g.drawText("FILTER", Rectangle<int>(10, 10, 220, 30), Justification::centred, false);
	
}

void FilterComponent::resized() {

	Rectangle<int> filterBounds(getLocalBounds().reduced(16));
	filterBounds.removeFromRight(15);
	__lpFilter->setBounds(filterBounds.removeFromRight(70).removeFromBottom(70));
	filterBounds.removeFromRight(25);
	__hpFilter->setBounds(filterBounds.removeFromRight(70).removeFromBottom(70));

}