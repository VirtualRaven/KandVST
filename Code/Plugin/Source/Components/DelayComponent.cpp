#include "DelayComponent.h"
#include "DelayComponent.h"

DelayComponent::~DelayComponent() {
}

DelayComponent::DelayComponent() :
	IVSTParameters(-1)
{
	__delaySpeed = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(0, "EX_DELAYMULTI"));
	__delaySeconds = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(0, "EX_DELAYLENGTH"));

	addAndMakeVisible(__delaySpeed);
	addAndMakeVisible(__delaySeconds);

	__delaySpeed->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__delaySpeed->setTextBoxStyle(__delaySpeed->TextBoxBelow, true, 50, 15);
	__speedLabel.setText("RATE", NotificationType::dontSendNotification);
	__speedLabel.attachToComponent(__delaySpeed, false);
	__speedLabel.setJustificationType(Justification:: centred);

	__delaySeconds->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__delaySeconds->setTextBoxStyle(__delaySpeed->TextBoxBelow, true, 50, 15);
	__secondsLabel.setText("TIME", NotificationType::dontSendNotification);
	__secondsLabel.attachToComponent(__delaySeconds, false);
	__secondsLabel.setJustificationType(Justification::centred);
}

void DelayComponent::paint(Graphics & g) {
	g.setColour(Colour::fromRGB(60, 60, 60));
	int width = __bounds.getWidth();
	int height = __bounds.getHeight();
	int fontHeight = __bounds.getHeight() * 0.2;
	int fontSize = __bounds.getHeight() * 0.16;

	g.drawRect(Rectangle<int>(10, 10, width, height), 2.0f);
	g.fillRect(Rectangle<int>(10, 10, width, fontHeight));

	g.setColour(Colours::white);
	g.setFont(Font(fontSize, Font::bold));
	g.drawText("DELAY", Rectangle<int>(10, 10, width, fontHeight), Justification::centred, false);
}

void DelayComponent::resized() {
	__bounds = getLocalBounds().reduced(5);
	int __size = (__bounds.getHeight() * 0.4);
	int gap = __size * 0.3;
	Rectangle<int> __delayBounds(__bounds);

	__delayBounds.removeFromLeft(gap * 1.2); 
	__delaySpeed->setBounds(__delayBounds.removeFromLeft(__size).removeFromBottom(__size + 15));
	__delayBounds.removeFromLeft(gap);
	__delaySeconds->setBounds(__delayBounds.removeFromLeft(__size).removeFromBottom(__size + 15));
}