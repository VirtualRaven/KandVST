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

#include "../JuceLibraryCode/JuceHeader.h"
#include "PresetRowModel.h"

PresetRowModel::PresetRowModel(ListBox* owner,GLOBAL*global):
	__owner(owner)
{
	Global = global;
}
PresetRowModel::~PresetRowModel()
{

}
void PresetRowModel::paintListBoxItem(int /*rowNumber*/, Graphics & /*g*/, int /*width*/, int /*height*/, bool /*rowIsSelected*/)
{

}

Component* PresetRowModel::refreshComponentForRow(int rowNumber, bool isRowSelected, Component * existingComponentToUpdate)
{
	delete existingComponentToUpdate;
	Component* R;
	
	if (rowNumber >= 0 && static_cast<size_t>(rowNumber) < Global->presetManager->GetPresetNames().size()) {
		R = new PresetRow(rowNumber, isRowSelected, __owner,Global);
	}
	else {
		R = new TextEditor();
		R->addKeyListener(this);
		
	}
	return R;
}
/*void PresetRowModel::listBoxItemDoubleClicked(int row, const MouseEvent &)
{
	if ( row > 0 &&  static_cast<size_t>(row) < Global->presetManager->GetPresetNames().size())
		Global->presetManager->LoadPreset(Global->presetManager->GetPresetNames()[row]);
}*/

bool PresetRowModel::keyPressed(const KeyPress & key, Component * originatingComponent)
{
	if (key == KeyPress::returnKey) {
		if (TextEditor * t = dynamic_cast<TextEditor*>(originatingComponent)) {
			if (t->getText() != "") {
				Global->presetManager->SavePreset(t->getText().toStdString());
				t->setText("");
				Global->presetManager->RefreshPresets();
				__owner->updateContent();
			}
			return true;
		}
	}
	return false;
}
