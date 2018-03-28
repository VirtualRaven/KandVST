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