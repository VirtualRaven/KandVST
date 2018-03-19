#ifndef PRESET_ROW
#define PRESET_ROW
#include "JuceHeader.h"
#include "../Global.h"
#include "../PresetManager.h"
class PresetRow : public ListBoxModel, private KeyListener
{
public:
	virtual Component* refreshComponentForRow(int rowNumber, bool isRowSelected, Component * existingComponentToUpdate) override;	
	int getNumRows()
	{
		return Global->presetManager->GetPresetNames().size() + 1;
	}
	virtual bool keyPressed(const KeyPress & key, Component * originatingComponent) override;
	virtual void listBoxItemDoubleClicked(int row, const MouseEvent & ev) override;
	PresetRow();
	~PresetRow();

	// Inherited via ListBoxModel
	virtual void paintListBoxItem(int rowNumber, Graphics & g, int width, int height, bool rowIsSelected) override;
};

#endif