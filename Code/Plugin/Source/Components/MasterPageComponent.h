#ifndef MASTER_COMPONENT_H
#define MASTER_COMPONENT_H

#include "../Global.h"
#include "../ParameterHandler.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "InformationComponent.h"
#include "PresetsComponent.h"
#include "ParameterSlider.h"
#include "../Global.h"
class MasterComponent : public Component
{
public:
    //==============================================================================
    MasterComponent ();
    ~MasterComponent();

    //==============================================================================

    void paint (Graphics& g) override;
    void resized() override;

private:
	InformationComponent __infoComponent;
	PresetsComponent __presetComponent;
	ScopedPointer<ParameterSlider> __hpFilter, __lpFilter;
	ScopedPointer<ParameterSlider> __osc1, __osc2, __osc3, __osc4, __masterMixer;
	Label __osc1Label, __osc2Label, __osc3Label, __osc4Label, __masterLabel, __mixerLabel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MasterComponent)
};
#endif