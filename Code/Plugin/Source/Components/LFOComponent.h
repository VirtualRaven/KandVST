#ifndef LFO_COMPONENT_H
#define LFO_COMPONENT_H

#include "../Global.h"
#include "../ParameterHandler.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "InformationComponent.h"
#include "ParameterSlider.h"
#include "../IVSTParameters.h"

class LFOComponent : public Component, private IVSTParameters, private Button::Listener
{
public:
    //==============================================================================
	LFOComponent(int ID);
    ~LFOComponent();

    //==============================================================================

    void paint (Graphics& g) override;
    void resized() override;
	void buttonClicked(Button* button) override;

private:
	//==============================================================================
	ScopedPointer<ParameterSlider> __ratio, __type, __amp;
	ToggleButton __toggleLfo;
	int __ID;
	bool lfoEnabled;
	//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOComponent)
};
#endif