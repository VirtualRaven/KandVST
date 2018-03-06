/*
==============================================================================

This file is part of the JUCE library.
Copyright (c) 2017 - ROLI Ltd.

JUCE is an open source library subject to commercial or open-source
licensing.

By using JUCE, you agree to the terms of both the JUCE 5 End-User License
Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
27th April 2017).

End User License Agreement: www.juce.com/juce-5-licence
Privacy Policy: www.juce.com/juce-5-privacy-policy

Or: You may also use this code under the terms of the GPL v3 (see
www.gnu.org/licenses).

JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
DISCLAIMED.

==============================================================================
*/


#include "ParameterSlider.h"



ParameterSlider::ParameterSlider(AudioProcessorParameter& p):
	 Slider(p.getName(256)), param(p)
{
	setRange(0.0, 1.0, 0.0);
	this->addMouseListener(this,false);
	startTimerHz(30);
	updateSliderPos();
	
}


ParameterSlider::~ParameterSlider()
{
}

void ParameterSlider::valueChanged()
{ 
	param.setValueNotifyingHost((float)Slider::getValue()); 
}

void ParameterSlider::timerCallback()
{ 
	updateSliderPos(); 
}

void ParameterSlider::startedDragging()
{ 
	param.beginChangeGesture(); 
}
void ParameterSlider::stoppedDragging()
{ 
	param.endChangeGesture(); 
}

double ParameterSlider::getValueFromText(const String& text)
{ 
	return param.getValueForText(text); 
}
String ParameterSlider::getTextFromValue(double value)
{ 
	return param.getText((float)value, 1024); 
}

void ParameterSlider::updateSliderPos()
{
	const float newValue = param.getValue();

	if (newValue != (float)Slider::getValue() && !isMouseButtonDown())
		Slider::setValue(newValue, NotificationType::dontSendNotification);
}

void ParameterSlider::mouseDoubleClick(const MouseEvent & event)
{
	param.setValueNotifyingHost(param.getDefaultValue());

}


