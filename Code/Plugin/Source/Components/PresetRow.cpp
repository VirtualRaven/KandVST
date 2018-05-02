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
#include "PresetRow.h"

PresetRow::PresetRow(int s, bool isSelected, ListBox* owner,GLOBAL*global):
	__isSelected(isSelected),
	__lbl("", global->presetManager->GetPresetNames()[s]),
	__rowNr(s),
	__owner(owner)
{

	Global = global;
	__lbl.addMouseListener(this, true);
	addAndMakeVisible(__lbl);

	// No save/del buttons for the Default preset
	if (__lbl.getText() != "Default")
	{
		__delete.addListener(this);
		__save.addListener(this);
		Image delImage = ImageFileFormat::loadFrom(Resources::Icons::delete2_png, sizeof(Resources::Icons::delete2_png));
		Image saveImage = ImageFileFormat::loadFrom(Resources::Icons::save2_png, sizeof(Resources::Icons::save2_png));
		__delete.setImages(false, true, true, delImage, 1.0f, Colours::transparentBlack, delImage, 0.7f, Colours::transparentBlack, delImage, 0.5f, Colours::transparentBlack);
		__save.setImages(false, true, true, saveImage, 1.0f, Colours::transparentBlack, saveImage, 0.7f, Colours::transparentBlack, saveImage, 0.5f, Colours::transparentBlack);
		if (isSelected) {
			addAndMakeVisible(__save);
			addAndMakeVisible(__delete);
		}
	}

}
PresetRow::~PresetRow()
{
}

/*bool PresetRow::keyPressed(const KeyPress & key, Component * originatingComponent) {
	if (key == KeyPress::returnKey) {
		if (TextEditor * t = dynamic_cast<TextEditor*>(originatingComponent)) {
			if (t->getText() != "") {
				Global->presetManager->SavePreset(t->getText().toStdString());
				t->setText("");
			}
			return true;
		}
	}
	return false;
}*/

void PresetRow::buttonClicked(Button * btn) {
	if (btn == &__save)
	{
		Global->presetManager->SavePreset(Global->presetManager->GetPresetNames()[__rowNr]);
		__owner->updateContent();
	}
	else if (btn == &__delete)
	{
		Global->presetManager->DeletePreset(Global->presetManager->GetPresetNames()[__rowNr]);
		__owner->updateContent();
	}
	
}

void PresetRow::mouseDoubleClick(const MouseEvent & event)
{
		Global->presetManager->LoadPreset(Global->presetManager->GetPresetNames()[__rowNr]);
}

void PresetRow::mouseDown(const MouseEvent & event)
{
	__owner->selectRow(__rowNr, false, true);

}

void PresetRow::resized()
{
	Rectangle<int> localBounds(getLocalBounds());
	__delete.setBounds(localBounds.removeFromRight(localBounds.getHeight()));
	__save.setBounds(localBounds.removeFromRight(localBounds.getHeight()));
	__lbl.setBounds(localBounds);
}







/*void PresetRow::listBoxItemClicked(int row, const MouseEvent &) 
{
	if (row > 0 && static_cast<size_t>(row) < Global->presetManager->GetPresetNames().size())
	{

	}	
}*/


