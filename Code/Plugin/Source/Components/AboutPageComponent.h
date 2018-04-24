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

#ifndef ABOUT_PAGE_COMPONENT_H
#define ABOUT_PAGE_COMPONENT_H

#include "JuceHeader.h"
#include "Global.h"
#include "ThemePicker.h"
#include "OurLookAndFeel.h"
#include "MarkdownComponent.h"
class AboutPageComponent : public Component, public ComboBox::Listener{
public :
	AboutPageComponent(GLOBAL * global);
	~AboutPageComponent();

	void paint(Graphics& g) override;
	void resized() override;
	virtual void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
	virtual void lookAndFeelChanged() override;

	static void RegisterParameters(int id, GLOBAL *global);
private:
	Viewport vp;
	MarkdownComponent* mc;
	OurLookAndFeel ourLookAndFeel;
	GLOBAL * Global;
	TextEditor __info;
	ComboBox __themes;
	AudioParameterChoice * __themeChoice;
	String __about;
	ThemePicker __themePicker;
	Colour __themeColour;
};



#endif ABOUT_PAGE_COMPONENT_H