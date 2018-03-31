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
