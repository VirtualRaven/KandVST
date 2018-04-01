#include "DelayComponent.h"
#include "DelayComponent.h"

DelayComponent::~DelayComponent() {
}

DelayComponent::DelayComponent(GLOBAL* global) :
	IVSTParameters(-1)
{
	Global = global;
	__delaySpeed = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(-1, "EX_DELAYLENGTH"), Global);
	__delaySeconds = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(-1, "EX_DELAYMULTI"), Global);

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

	g.drawRect(Rectangle<int>(0, 0, width, height), 2.0f);
	g.fillRect(Rectangle<int>(0, 0, width, fontHeight));

	g.setColour(Colours::white);
	g.setFont(Font(fontSize, Font::bold));
	g.drawText("DELAY", Rectangle<int>(0, 0, width, fontHeight), Justification::centred, false);
}

void DelayComponent::resized() {
	__bounds = getLocalBounds();
	
	int fontHeight = __bounds.getHeight() * 0.2;
	Rectangle<int> __delayBounds(__bounds.reduced(8));

	__delayBounds.removeFromTop(fontHeight);

	__delayBounds.removeFromTop(__secondsLabel.getFont().getHeight());

	int __size = (__delayBounds.getWidth() * 0.5f);
	__delaySpeed->setBounds(__delayBounds.removeFromLeft(__size));
	__delaySeconds->setBounds(__delayBounds);
}