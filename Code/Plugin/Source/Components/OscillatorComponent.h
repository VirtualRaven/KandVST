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
#include "../ParameterListener.h"
class OscillatorComponent : public Component, private IVSTParameters, private Timer, private ParameterListener
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

	Label __sineLabel, __squareLabel, __sawLabel, __triangleLabel,__noiseLabel;
	ScopedPointer<ParameterSlider> __sineSlider, __squareSlider, __sawSlider, __triangleSlider,__noiseSlider;

	Label __delayLabel, __octaveLabel, __detuneLabel, __offsetLabel, __overtoneLabel;
	ScopedPointer<ParameterSlider> __delaySlider, __octaveSlider, __detuneSlider, __offsetSlider, __overtoneSlider;
	Image* __oscWaveform;
	ImageComponent __waveformComp;
	bool __waveformInvalid;

	//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscillatorComponent)

		// Inherited via Timer
		virtual void timerCallback() override;

	// Inherited via ParameterListener
	virtual void parametersChanged(std::vector<std::string>) override;
};
#endif