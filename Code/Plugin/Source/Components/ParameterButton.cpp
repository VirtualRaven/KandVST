#include "ParameterButton.h"


ParameterButton::~ParameterButton()
{
}

ParameterButton::ParameterButton(AudioProcessorParameter& p)
	: TextButton(p.getName(256)), param(p)
{
	setValue(static_cast<int>(param.getValue())); //Set Default value
	param.setValueNotifyingHost(static_cast<float>(__value));
	setColour(TextButton::ColourIds::buttonOnColourId, Colours::green);
	setColour(TextButton::ColourIds::buttonColourId, Colour::fromRGB(60,60,60));
	startTimer(30);
	addListener(this);

}

void ParameterButton::setValue(int value) {
	__value = value;
	param.setValue(static_cast<float>(__value));
	setToggleState(__value, juce::NotificationType::dontSendNotification);
	param.setValueNotifyingHost(static_cast<float>(__value));
}

void ParameterButton::timerCallback()
{
	if (!lock.try_lock())
		return;
	else
	{
		if (__value != param.getValue()) {
			setValue(static_cast<int>(param.getValue()));
		}
		lock.unlock();
	}
}

int ParameterButton::getValue() {
	return __value;
}

void ParameterButton::buttonClicked(Button* ) {
	lock.lock();
	if (getValue())
		setValue(0);
	else
		setValue(1);
	
	lock.unlock();
}