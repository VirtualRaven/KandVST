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


#include "ParameterButton.h"


ParameterButton::~ParameterButton()
{
}

ParameterButton::ParameterButton(AudioProcessorParameter& p)
	: ToggleButton(p.getName(256)), param(p)
{
	__value = 0;
	param.setValueNotifyingHost((bool)__value);
	setColour(ToggleButton::tickDisabledColourId, Colours::darkgrey);
}

void ParameterButton::setValue(int value) {
	__value = value;
}

int ParameterButton::getValue() {
	return __value;
}

void ParameterButton::buttonClicked(Button* button) {
	if (param.getValue()) { // if value = 1 (enabled)
		setValue(0);
		setColour(ToggleButton::tickDisabledColourId, Colours::darkgrey);
	} else { //disabled
		setValue(1);
		setColour(ToggleButton::tickColourId, Colours::skyblue);
	}
	param.setValueNotifyingHost((bool)__value);
}