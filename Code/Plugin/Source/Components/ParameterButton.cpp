/*
 * This file is part of the KandVST synthesizer.
 *
 * Copyright (c) 2018   Lukas Rahmn, Anton Fredriksson,
 *                      Sarosh Nasir, Stanisław Zwierzchowski,
 *                      Klas Ludvigsson and Andreas Kuszli
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "ParameterButton.h"


ParameterButton::~ParameterButton()
{
}

ParameterButton::ParameterButton(AudioProcessorParameter& p)
	: TextButton(p.getName(256)), param(p)
{
	setValue(static_cast<int>(param.getValue())); //Set Default value
	param.setValueNotifyingHost(static_cast<float>(__value));
	//setColour(TextButton::ColourIds::buttonOnColourId, Colour::fromRGB(60,80,110));
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