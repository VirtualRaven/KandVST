#include "PresetRow.h"

PresetRow::PresetRow()
{
}


PresetRow::~PresetRow()
{
}

void PresetRow::paintListBoxItem(int rowNumber, Graphics & g, int width, int height, bool rowIsSelected)
{
}

Component* PresetRow::refreshComponentForRow(int rowNumber, bool isRowSelected, Component * existingComponentToUpdate)
{
	Component* l;
	delete existingComponentToUpdate;
	if (rowNumber > 0 && static_cast<size_t>( rowNumber) < Global->presetManager->GetPresetNames().size()) {
		l = new Label("", Global->presetManager->GetPresetNames()[rowNumber]);
		l->setInterceptsMouseClicks(false, false);
		if (isRowSelected)
			l->setColour(Label::backgroundColourId, Colours::green);
	}
	else {
		l = new TextEditor();
		l->addKeyListener(this);
	}

	return l;
}

bool PresetRow::keyPressed(const KeyPress & key, Component * originatingComponent) {
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
}

void PresetRow::listBoxItemDoubleClicked(int row, const MouseEvent &)
{
	if ( row > 0 &&  static_cast<size_t>(row) < Global->presetManager->GetPresetNames().size())
		Global->presetManager->LoadPreset(Global->presetManager->GetPresetNames()[row]);
}
