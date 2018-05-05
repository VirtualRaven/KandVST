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

#ifndef OSCILLATOR_PAGE_COMPONENT_H
#define OSCILLATOR_PAGE_COMPONENT_H
#include "../JuceLibraryCode/JuceHeader.h"
#include "IVSTParameters.h"
#include "EnvelopeComponent.h"
#include "ParameterSlider.h"
#include "Global.h"
#include "OscillatorComponent.h"
#include "LFOComponent.h"
#include "FilterComponent.h"
#include "OSCLFOComponent.h"
#include "DistComponent.h"
#include "Swatch.h"

class OscillatorPageComponent  : public Component, private IVSTParameters
{
public:
    //==============================================================================
    OscillatorPageComponent (int ID,GLOBAL * global);
    ~OscillatorPageComponent();

    //==============================================================================

    void paint (Graphics& g) override;
    void resized() override;
	GLOBAL * Global;
private:
    //==============================================================================
	EnvelopeComponent __envComponent;
	OscillatorComponent __oscillator;
	DistComponent __dist;
	ComboBox __oscOctave;
	ScopedPointer<ParameterSlider> __amplitude;
	Label __ampLabel;
	ScopedPointer<ParameterButton> __toggleOsc;

	FilterComponent __filterComponent;
	OSCLFOComponent __osclfoComponent;
    //==============================================================================
    
};
#endif