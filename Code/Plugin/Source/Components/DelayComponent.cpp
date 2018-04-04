#include "DelayComponent.h"
#include "DelayComponent.h"

DelayComponent::~DelayComponent() {
}

DelayComponent::DelayComponent(GLOBAL* global) :
	IVSTParameters(-1)
{
	Global = global;
	__delaySpeed = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "EX_DELAYLENGTH"), Global);
	__delaySeconds = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "EX_DELAYMULTI"), Global);
	__toggleDelay = new ParameterButton(*Global->paramHandler->Get<AudioParameterBool>(__ID, "DELAY_EN"));

	addAndMakeVisible(__delaySpeed);
	addAndMakeVisible(__delaySeconds);
	addAndMakeVisible(__toggleDelay);

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

	__toggleDelay->setButtonText("DELAY");

}

void DelayComponent::paint(Graphics & g) {
	g.setColour(Colour::fromRGB(60, 60, 60));
	int width = getLocalBounds().getWidth();
	int height = getLocalBounds().getHeight();
	int fontHeight = getLocalBounds().getHeight() * 0.2;
	int fontSize = getLocalBounds().getHeight() * 0.16;

	g.drawRect(Rectangle<int>(0, 0, width, height), 2.0f);
	g.fillRect(Rectangle<int>(0, 0, width, fontHeight));

	// if enabled
	if (*Global->paramHandler->Get<AudioParameterBool>(__ID, "DELAY_EN")) {
		__delaySpeed->setEnabled(true);
		__delaySeconds->setEnabled(true);
		
	}
	else //disabled
	{ 
		__delaySpeed->setEnabled(false);
		__delaySeconds->setEnabled(false);
	}

}

void DelayComponent::resized() {
	__bounds = getLocalBounds();
	
	int fontHeight = __bounds.getHeight() * 0.2;
	__toggleDelay->setBounds(__bounds.removeFromTop(fontHeight));

	Rectangle<int> __delayBounds(__bounds.reduced(8));
	__delayBounds.removeFromTop(__secondsLabel.getFont().getHeight());

	int __size = (__delayBounds.getWidth() * 0.5f);
	__delaySpeed->setBounds(__delayBounds.removeFromLeft(__size));
	__delaySeconds->setBounds(__delayBounds);
}