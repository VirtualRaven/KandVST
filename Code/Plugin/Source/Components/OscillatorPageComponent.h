#ifndef OSCILLATOR_PAGE_COMPONENT_H
#define OSCILLATOR_PAGE_COMPONENT_H
#include "../JuceLibraryCode/JuceHeader.h"
#include "../IVSTParameters.h"
#include "EnvelopeComponent.h"
#include "ParameterSlider.h"
#include "../Global.h"
#include "OscillatorComponent.h"
#include "LFOComponent.h"
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
	LFOComponent		__lfo;
	ComboBox __oscOctave;
	

        
    //==============================================================================
    
};
#endif