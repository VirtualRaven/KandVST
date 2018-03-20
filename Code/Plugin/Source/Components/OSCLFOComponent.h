#ifndef OSCLFO_COMPONENT_H
#define OSCLFO_COMPONENT_H

#include "../Global.h"
#include "../ParameterHandler.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "InformationComponent.h"
#include "SelectKnob.h"
#include "ParameterButton.h"
#include "../IVSTParameters.h"

class OSCLFOComponent : public Component, private IVSTParameters
{
public:
    //==============================================================================
	OSCLFOComponent(int ID);
    ~OSCLFOComponent();

    //==============================================================================

    void paint (Graphics& g) override;
    void resized() override;

private:
	//==============================================================================
	ScopedPointer<SelectKnob> __freqSelcet, __ampSelect;
	Label __freqLabel, __ampLabel;
	Rectangle<int> __bounds;
	int __ID, __width, __height;
	//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCLFOComponent)
};
#endif