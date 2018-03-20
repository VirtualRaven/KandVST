#ifndef PRESETS_COMPONENT_H
#define PRESETS_COMPONENT_H

#include "../Global.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "../Resources_files.h"
#include "PresetRow.h"
#include "../OurLookAndFeel.h"

class PresetsComponent  : public Component, private Button::Listener
{
public:
	PresetsComponent();
	~PresetsComponent();
    void resized() override;
private:
	ImageButton __refresh;
	ListBox __presets;
	PresetRow __rowModel;
	// Inherited via Listener
	virtual void buttonClicked(Button *) override;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetsComponent)
};

#endif // !PRESETS_COMPONENT_H
