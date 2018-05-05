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

#ifndef PARAMETER_BUTTON_H
#define PARAMETER_BUTTON_H
#include <mutex>
#include "../JuceLibraryCode/JuceHeader.h"
class ParameterButton : public TextButton, private Button::Listener, private Timer
{
public:
	ParameterButton(AudioProcessorParameter& p);
	~ParameterButton();

	void buttonClicked(Button* button) override;
	int getValue();
	void setValue(int value);
	AudioProcessorParameter& param;
	std::mutex lock;
private:
	int __value;
	// Inherited via Timer
	virtual void timerCallback() override;
	// 1 = on, 0 = off
};
#endif
