#pragma once

#include "../Global.h"
#include "../ParameterHandler.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "InformationComponent.h"

class CollectionComponent  : public Component
{
public:
    //==============================================================================
    CollectionComponent ();
    ~CollectionComponent();

    //==============================================================================

    void paint (Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    InformationComponent infoWindow;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CollectionComponent)
};