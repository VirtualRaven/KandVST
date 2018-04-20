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