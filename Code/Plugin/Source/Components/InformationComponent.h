#pragma once

#include "../Global.h"
#include "../ParameterHandler.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "../JuceLibraryCode/JuceHeader.h"

class InformationComponent  : public Component
{
public:
    //==============================================================================
    InformationComponent ();
    ~InformationComponent();

    //==============================================================================

    void paint (Graphics& g) override;
    void resized() override;
private:
    //==============================================================================
    Label __version, __authors;
	int x, y, midX, midY, w, h, offset;
	float startAlpha, endAlpha;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InformationComponent)
};