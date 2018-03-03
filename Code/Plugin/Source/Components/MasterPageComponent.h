#ifndef MASTER_COMPONENT_H
#define MASTER_COMPONENT_H

#include "../Global.h"
#include "../ParameterHandler.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "InformationComponent.h"
#include "PresetsComponent.h"
#include "ParameterSlider.h"
#include "../Global.h"
#include "MixerComponent.h"
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
	MixerComponent __mixerComponent;
	
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MasterComponent)
};
#endif