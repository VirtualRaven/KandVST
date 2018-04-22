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

#ifndef OSCLFO_COMPONENT_H
#define OSCLFO_COMPONENT_H

#include "Global.h"
#include "ParameterHandler.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "InformationComponent.h"
#include "ParameterButton.h"
#include "IVSTParameters.h"

class OSCLFOComponent : public Component, private IVSTParameters
{
public:
    //==============================================================================
	OSCLFOComponent(int ID,GLOBAL * global);
    ~OSCLFOComponent();

    //==============================================================================

    void paint (Graphics& g) override;
    void resized() override;
	GLOBAL * Global;
private:
	//==============================================================================
	ScopedPointer<ParameterSlider> __freqSelect, __ampSelect, __lpSelect, __hpSelect;
	Label __freqLabel, __ampLabel, __lpLabel, __hpLabel;
	Rectangle<int> __bounds;
	int __ID, __width, __height;
	//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCLFOComponent)
};
#endif