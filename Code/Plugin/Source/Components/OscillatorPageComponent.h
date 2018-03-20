#ifndef OSCILLATOR_PAGE_COMPONENT_H
#define OSCILLATOR_PAGE_COMPONENT_H
#include "../JuceLibraryCode/JuceHeader.h"
#include "../IVSTParameters.h"
#include "EnvelopeComponent.h"
#include "ParameterSlider.h"
#include "../Global.h"
#include "OscillatorComponent.h"
#include "LFOComponent.h"
#include "FilterComponent.h"
#include "OSCLFOComponent.h"
#include "../Swatch.h"
class OscillatorPageComponent  : public Component, private IVSTParameters
{
public:
    //==============================================================================
    OscillatorPageComponent (int ID);
    ~OscillatorPageComponent();

    //==============================================================================

    void paint (Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
	EnvelopeComponent __envComponent;
	OscillatorComponent __oscillator;
	ComboBox __oscOctave;
	ScopedPointer<ParameterSlider> __amplitude;
	Label __ampLabel;
	ScopedPointer<ParameterButton> __toggleOsc;

	FilterComponent __filterComponent;
	OSCLFOComponent __osclfoComponent;
    //==============================================================================
    
};
#endif