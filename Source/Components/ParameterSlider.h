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



#ifndef PARAMETER_SLIDER_H
#define PARAMETER_SLIDER_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "Global.h"
#include "Linkable.h"
#include "ParameterHandler.h"

class ParameterSlider: public Slider, private Timer, private Linkable
{
public:
	enum ProgressStart {
		Disabled,
		Start,
		End,
		Center,
		DotsOnly,
		SingleDot
	};
private:
	virtual void mouseUp(const MouseEvent& event) override;
	Point<float> __mousePos;
	ProgressStart __progressStart;
	int __wave; // 1  = sine, 2 = square, 3 = saw, 4 = triangle, 5 = noise; 0 = no wave

public:
	ParameterSlider(AudioProcessorParameter& p,GLOBAL*global);
	~ParameterSlider();

	void setWaveType(int);
	int getWaveType();
	void valueChanged() override;

	void timerCallback() override;

	void startedDragging() override;
	void stoppedDragging() override;

	void setDrawProgress(ProgressStart startLocation = ProgressStart::Start);
	ProgressStart getDrawProgress();
	double getValueFromText(const String& text) override;
	String getTextFromValue(double value) override;

	void updateSliderPos();
	AudioProcessorParameter& param;
	virtual void mouseDoubleClick(const MouseEvent& event) override;

	void Link(int id);
	void RemoveLink();

	// Inherited via Linkable
	virtual void LinkCouldHaveChanged() override;

	GLOBAL*Global;

};


#endif
