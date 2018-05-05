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

#ifndef OUR_TABBED_BUTTON_BAR_H
#define OUR_TABBED_BUTTON_BAR_H


#include "../JuceLibraryCode/JuceHeader.h"

class OurTabbedButtonBar;

class OurTabBarButton : public TabBarButton {
public:
	void paintButton(Graphics&, bool isMouseOverButton, bool isButtonDown) override;
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OurTabBarButton)
};

class OurTabbedButtonBar : public TabbedButtonBar
{
public:
	virtual OurTabBarButton* createTabButton(const String& tabName, int tabIndex) override;

private:


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OurTabbedButtonBar)
};

#endif
