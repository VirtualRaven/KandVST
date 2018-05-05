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

#ifndef MASTER_COMPONENT_H
#define MASTER_COMPONENT_H

#include "Global.h"
#include "ParameterHandler.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "InformationComponent.h"
#include "PresetsComponent.h"
#include "ParameterSlider.h"
#include "MixerComponent.h"
#include "FilterComponent.h"
#include "LFOComponent.h"
#include "SettingsComponent.h"
#include "DelayComponent.h"
#include "ReverbComponent.h"
#include "Swatch.h"
class MasterComponent : public Component
{
public:
    //==============================================================================
    MasterComponent (GLOBAL*global);
    ~MasterComponent();

    //==============================================================================

    void paint (Graphics& g) override;
    void resized() override;
	GLOBAL * Global;
private:
	InformationComponent __infoComponent;
	MixerComponent __mixerComponent;
	FilterComponent __filterComponent;
	LFOComponent __lfoComponent1, __lfoComponent2;
	DelayComponent __delayComponent;
	ReverbComponent __reverbComponent;

	// Temporary on master
	SettingsComponent __settingsComponent;

	
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MasterComponent)
};
#endif