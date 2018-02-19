#ifndef OSCILLATOR_COMPONENT_H
#define OSCILLATOR_COMPONENT_H

#include "../Global.h"
#include "../ParameterHandler.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "InformationComponent.h"
#include "ParameterSlider.h"
#include "ParameterButton.h"
#include "../IVSTParameters.h"
#include "../WavetableOsc.h"

class OscillatorComponent : public Component, private IVSTParameters, private Timer, private Button::Listener
{
public:
    //==============================================================================
    OscillatorComponent (int ID);
    ~OscillatorComponent();

    //==============================================================================

    void paint (Graphics& g) override;
    void resized() override;
	void buttonClicked(Button* button) override;

private:
	//==============================================================================

	Label __sineLabel, __squareLabel, __sawLabel, __triangleLabel;
	ScopedPointer<ParameterSlider> __sineSlider, __squareSlider, __sawSlider, __triangleSlider;

	Label __delayLabel, __octaveLabel, __detuneLabel, __offsetLabel;
	ScopedPointer<ParameterSlider> __delaySlider, __octaveSlider, __detuneSlider, __offsetSlider;
	float s, sq, sa, tr;
	Image* __oscWaveform;
	ScopedPointer<ParameterButton> __toggleOsc;
	int __ID;

	//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscillatorComponent)

		// Inherited via Timer
		virtual void timerCallback() override;
};
#endif