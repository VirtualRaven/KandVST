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

#ifndef OUR_LOOK_AND_FEEL_H
#define OUR_LOOK_AND_FEEL_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "Resources_files.h"
#include "Swatch.h"
#include "Global.h"
#include "Components/ParameterSlider.h"
#include "ThemePicker.h"
class OurLookAndFeel : public LookAndFeel_V4
{

public:
	 OurLookAndFeel(GLOBAL * global);
	~OurLookAndFeel();
	void drawRotarySlider(Graphics& g, int x, int y, int width, int height,
		float sliderPos, float rotaryStartAngle,  float rotaryEndAngle, Slider& slider) override;
	
	void drawLinearSlider(Graphics& g, int x, int y, int width, int height,
		float sliderPos, float minSliderPos, float maxSliderPos, Slider::SliderStyle style, Slider& slider) override;

	void drawTabButton(TabBarButton&, Graphics&, bool isMouseOverButton, bool isMouseDown) override;

	void drawButtonBackground(Graphics&, Button&, const Colour& backgroundColour,bool isMouseOverButton, bool isButtonDown) override;

	static void createTabTextLayout(const TabBarButton& button, float length, float depth, Colour colour, TextLayout&);
	
	void drawButtonText(Graphics&, TextButton&, bool isMouseOverButton, bool isButtonDown) override;

private :
	GLOBAL * Global;
	Colour __themeColour;
	ThemePicker __themePicker;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OurLookAndFeel)
};

#endif
