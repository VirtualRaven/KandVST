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

#ifndef OSCILLATOR_COMPONENT_H
#define OSCILLATOR_COMPONENT_H

#include "Global.h"
#include "ParameterHandler.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "InformationComponent.h"
#include "ParameterSlider.h"
#include "ParameterButton.h"
#include "IVSTParameters.h"
#include "WavetableOsc.h"
#include "ParameterListener.h"
class OscillatorComponent : public Component, private IVSTParameters, private Timer, private ParameterListener, private ComponentListener
{
public:
    //==============================================================================
    OscillatorComponent (int ID,GLOBAL*global);
    ~OscillatorComponent();

    //==============================================================================

    void paint (Graphics& g) override;
    void resized() override;
	GLOBAL * Global;
private:
	//==============================================================================
	Label __sineLabel, __squareLabel, __sawLabel, __triangleLabel,__noiseLabel;
	ScopedPointer<ParameterSlider> __sineSlider, __squareSlider, __sawSlider, __triangleSlider,__noiseSlider;

	Label __delayLabel, __octaveLabel, __detuneLabel, __offsetLabel, __overtoneLabel;
	ScopedPointer<ParameterSlider> __delaySlider, __octaveSlider, __detuneSlider, __offsetSlider, __overtoneSlider;
	Image* __oscWaveform;
	ImageComponent __waveformComp;
	bool __waveformInvalid;
	virtual void componentVisibilityChanged(Component &component) override;
	virtual void componentParentHierarchyChanged(Component &component) override;
	virtual void lookAndFeelChanged() override;
	//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscillatorComponent)

		// Inherited via Timer
		virtual void timerCallback() override;

	// Inherited via ParameterListener
	virtual void parametersChanged(std::vector<std::string>) override;
};
#endif