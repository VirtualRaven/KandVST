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

#ifndef PRESET_ROW
#define PRESET_ROW
#include "JuceHeader.h"
#include "Global.h"
#include "PresetManager.h"
#include "Resources_files.h"
class PresetRow : public Component, private Button::Listener

{
private:
	virtual void resized() override;
	bool __isSelected;
	int __rowNr;
	// Inherited via Listener
	virtual void buttonClicked(Button *) override;
	// Inherited via MouseListener
	virtual void mouseDoubleClick(const MouseEvent& event) override;
	virtual void mouseDown(const MouseEvent& event) override;
	void paint(Graphics& g) override;
	ListBox* __owner;

public:
	Label __lbl;
	ImageButton __save;
	ImageButton __delete;
	
	PresetRow(int s, bool isSelected, ListBox* owner,GLOBAL*global);
	~PresetRow();
	GLOBAL * Global;
	
};

#endif