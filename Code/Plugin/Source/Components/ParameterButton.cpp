#include "ParameterButton.h"


ParameterButton::~ParameterButton()
{
}

ParameterButton::ParameterButton(AudioProcessorParameter& p)
	: TextButton(p.getName(256)), param(p)
{
	setValue(param.getValue()); //Set Default value
	param.setValueNotifyingHost(__value);
	setColour(TextButton::ColourIds::buttonOnColourId, Colours::green);
	setColour(TextButton::ColourIds::buttonColourId, Colours::darkgrey);
	//setColour(ToggleButton::tickDisabledColourId, Colours::darkgrey);
	//setColour(ToggleButton::tickColourId, Colours::skyblue);
	startTimer(30);
	addListener(this);

}

void ParameterButton::setValue(int value) {
	__value = value;
	param.setValue(__value);
	setToggleState(__value, juce::NotificationType::dontSendNotification);
	param.setValueNotifyingHost(__value);
}

void ParameterButton::timerCallback()
{
	if (!lock.try_lock())
		return;
	else
	{
		if (__value != param.getValue()) {
			setValue(param.getValue());
		}
		lock.unlock();
	}
}

int ParameterButton::getValue() {
	return __value;
}

void ParameterButton::buttonClicked(Button* button) {
	lock.lock();
	if (getValue())
		setValue(0);
	else
		setValue(1);
	
	lock.unlock();
}