#pragma once

#include "../Global.h"
#include "../ParameterHandler.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "../JuceLibraryCode/JuceHeader.h"

class OscillatorComponent  : public Component
{
public:
    //==============================================================================
    OscillatorComponent ();
    ~OscillatorComponent();

    //==============================================================================

    void paint (Graphics& g) override;
    void resized() override;

private:
    //==============================================================================


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscillatorComponent)
};