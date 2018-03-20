#ifndef LFO_COMPONENT_H
#define LFO_COMPONENT_H

#include "Global.h"
#include "ParameterHandler.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "InformationComponent.h"
#include "ParameterSlider.h"
#include "ParameterButton.h"
#include "IVSTParameters.h"

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
	ScopedPointer<ParameterSlider> __ratio, __type, __amp;
	Label __ratioLabel, __typeLabel, __ampLabel;
	ScopedPointer<ParameterButton> __toggleLfo;
	Rectangle<int> __bounds;
	int __ID, __width, __height;
	//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOComponent)
};
#endif