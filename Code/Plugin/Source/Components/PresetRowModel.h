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

#ifndef PRESET_ROW_MODEL_H
#define PRESET_ROW_MODEL_H
#include "Global.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "Resources_files.h"
#include "../Plugin/Source/Components/PresetRow.h"
class PresetRowModel : public ListBoxModel, private KeyListener
{
public:
	PresetRowModel(ListBox* owner,GLOBAL*global);
	~PresetRowModel();
	virtual Component* refreshComponentForRow(int rowNumber, bool isRowSelected, Component * existingComponentToUpdate) override;
	//virtual void listBoxItemClicked(int row, const MouseEvent & ev) override;
	//virtual void listBoxItemDoubleClicked(int row, const MouseEvent & ev) override;
	virtual void paintListBoxItem(int rowNumber, Graphics & g, int width, int height, bool rowIsSelected) override;
	GLOBAL * Global;

private:
	ListBox * __owner;
	// Inherited via ListBoxModel
	virtual int getNumRows() override
	{
		return Global->presetManager->GetPresetNames().size() + 1;
	} 
	virtual bool keyPressed(const KeyPress & key, Component * originatingComponent) override;
};




#endif //!PRESET_ROW_MODEL_H