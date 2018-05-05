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

#ifndef LFO_COMPONENT_H
#define LFO_COMPONENT_H

#include "Global.h"
#include "ParameterHandler.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterSlider.h"
#include "ParameterButton.h"
#include "IVSTParameters.h"
#include "Swatch.h"

class LFOComponent : public Component, private IVSTParameters, public ComboBox::Listener
{
public:
    //==============================================================================
	LFOComponent(int ID,GLOBAL*global);
    ~LFOComponent();

    //==============================================================================

    void paint (Graphics& g) override;
    void resized() override;
	GLOBAL * Global;
	virtual void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
private:
	//==============================================================================
	ScopedPointer<ParameterSlider> __type, __amp;
	Label __ratioLabel, __typeLabel, __ampLabel;
	ScopedPointer<ParameterButton> __toggleLfo, __toggleInvert, __onPress;
	Rectangle<int> __bounds;
	int __ID, __width, __height;
	ComboBox __cBox;
	StringArray __choices;
	AudioParameterChoice * __choiceParameter;
	//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOComponent)
};
#endif