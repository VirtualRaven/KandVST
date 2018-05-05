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

#ifndef SETTINGS_COMPONENT_H
#define SETTINGS_COMPONENT_H

#include "Global.h"
#include "ParameterSlider.h"
#include "../JuceLibraryCode/JuceHeader.h"

class SettingsComponent : public Component
{
public:
	SettingsComponent(GLOBAL*global);
	~SettingsComponent();
	static void RegisterParameters(int ID, GLOBAL*Global);
	GLOBAL * Global;
private:
	void paint(Graphics& g) override;
	void resized() override;

	ScopedPointer<ParameterSlider> __pitchBendSens, __selectKnob;
	Label __pitchBendLabel;
	Rectangle<int> __bounds;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsComponent)
};

#endif //SETTINGS_COMPONENT_H
