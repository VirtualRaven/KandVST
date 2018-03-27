#ifndef MASTER_COMPONENT_H
#define MASTER_COMPONENT_H

#include "Global.h"
#include "ParameterHandler.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "InformationComponent.h"
#include "PresetsComponent.h"
#include "ParameterSlider.h"
#include "MixerComponent.h"
#include "FilterComponent.h"
#include "LFOComponent.h"
#include "SettingsComponent.h"
#include "DelayComponent.h"
#include "Swatch.h"
class MasterComponent : public Component
{
public:
    //==============================================================================
    MasterComponent (GLOBAL*global);
    ~MasterComponent();

    //==============================================================================

    void paint (Graphics& g) override;
    void resized() override;
	GLOBAL * Global;
private:
	InformationComponent __infoComponent;
	MixerComponent __mixerComponent;
	FilterComponent __filterComponent;
	LFOComponent __lfoComponent1, __lfoComponent2;
	DelayComponent __delayComponent;

	// Temporary on master
	SettingsComponent __settingsComponent;

	
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MasterComponent)
};
#endif