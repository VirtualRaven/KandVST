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

#ifndef MIXER_SUB_COMPONENT_H
#define MIXER_SUB_COMPONENT_H

#include "Global.h"
#include "ParameterSlider.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "IVSTParameters.h"
class MixerSubComponent : public Component,IVSTParameters
{
public:
	MixerSubComponent(int ID,String  label, GLOBAL*global);
	MixerSubComponent(int ID, String  label, String overridePanId,String overrideGainId, bool isMaster,GLOBAL*global);
	~MixerSubComponent();
	GLOBAL * Global;
private: 
	void paint(Graphics& g) override;
	void resized() override;

	bool __isMaster;

	ScopedPointer<ParameterSlider> __gain;
	Label __label;

	ScopedPointer<ParameterSlider> __pan;
	Label __panL, __panR;
	Rectangle<int> __bounds;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MixerSubComponent)
};

#endif //MIXER_SUB_COMPONENT_H
