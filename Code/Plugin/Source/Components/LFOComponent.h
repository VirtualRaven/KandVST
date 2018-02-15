#ifndef OSCILLATOR_COMPONENT_H
#define OSCILLATOR_COMPONENT_H

#include "../Global.h"
#include "../ParameterHandler.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "InformationComponent.h"
#include "ParameterSlider.h"
#include "../IVSTParameters.h"

class LFOComponent : public Component, private IVSTParameters
{
public:
    //==============================================================================
	LFOComponent(int ID);
    ~LFOComponent();

    //==============================================================================

    void paint (Graphics& g) override;
    void resized() override;

private:
	//==============================================================================

	

	//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOComponent)
};
#endif