#include "../JuceLibraryCode/JuceHeader.h"
#include "PresetRow.h"

PresetRow::PresetRow(int s, bool isSelected, ListBox* owner):
	__isSelected(isSelected),
	__lbl("", Global->presetManager->GetPresetNames()[s]),
	__rowNr(s),
	__owner(owner)
{
	__lbl.addMouseListener(this, true);
	addAndMakeVisible(__lbl);
	__delete.addListener(this);
	__save.addListener(this);
	Image delImage = ImageFileFormat::loadFrom(Resources::Icons::delete_png, sizeof(Resources::Icons::delete_png));
	Image saveImage = ImageFileFormat::loadFrom(Resources::Icons::save_png, sizeof(Resources::Icons::save_png));
	__delete.setImages(false, true, true, delImage, 1.0f, Colours::transparentBlack, delImage, 0.7f, Colours::transparentBlack, delImage, 0.5f, Colours::transparentBlack);
	__save.setImages(false, true, true, saveImage, 1.0f, Colours::transparentBlack, saveImage, 0.7f, Colours::transparentBlack, saveImage, 0.5f, Colours::transparentBlack);
	if (isSelected) {
		addAndMakeVisible(__save);
		addAndMakeVisible(__delete);
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


