#ifndef OSCILLATOR_COMPONENT_H
#define OSCILLATOR_COMPONENT_H
#include "../JuceLibraryCode/JuceHeader.h"
#include "../IVSTParameters.h"
#include "EnvelopeComponent.h"

class OscillatorComponent  : public Component, private IVSTParameters
{
public:
    //==============================================================================
    OscillatorComponent (int ID);
    ~OscillatorComponent();

    //==============================================================================

    void paint (Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
	EnvelopeComponent __envComponent;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscillatorComponent)
};
#endif