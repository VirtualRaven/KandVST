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

#ifndef REVERB_COMPONENT_H
#define REVERB_COMPONENT_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "Global.h"
#include "ConvolutionReverb.h"
#include "ParameterSlider.h"
#include "ParameterButton.h"
#include "IVSTParameters.h"
#include "Swatch.h"

class ReverbComponent : public Component, public ComboBox::Listener, private IVSTParameters
 {
public:
	ReverbComponent(int ID, GLOBAL*global);
	~ReverbComponent();

	void paint(Graphics& g) override;
	void resized() override;
	virtual void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;

private:
	ScopedPointer<ParameterSlider> __dry, __wet;
	ScopedPointer<ParameterButton> __toggleReverb;
	AudioParameterChoice *__irChoice;
	Label __dryLabel, __wetLabel, __boxLabel;
	ComboBox __cBox;
	GLOBAL * Global;
	StringArray __ir;
};

#endif REVERB_COMPONENT_H
