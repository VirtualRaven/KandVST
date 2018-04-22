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

#ifndef PRESETS_COMPONENT_H
#define PRESETS_COMPONENT_H

#include "Global.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "Resources_files.h"
#include "PresetRow.h"
#include "OurLookAndFeel.h"
#include "PresetRowModel.h"

class PresetsComponent  : public Component, private Button::Listener

{
public:
	PresetsComponent(GLOBAL * global);
	~PresetsComponent();
    void resized() override;
	GLOBAL * Global;
private:
	ImageButton __refresh;
	ListBox __presets;
	PresetRowModel __rowModel;
	// Inherited via Listener
	virtual void buttonClicked(Button *) override;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetsComponent)
};

#endif // !PRESETS_COMPONENT_H
	