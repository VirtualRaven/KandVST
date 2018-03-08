#ifndef MIXER_COMPONENT_H
#define MIXER_COMPONENT_H

#include "../Global.h"
#include "ParameterSlider.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "../JuceLibraryCode/JuceHeader.h"

class MixerComponent : public Component
{
public:
	MixerComponent();
	~MixerComponent();

private: 
	void paint(Graphics& g) override;
	void resized() override;

	ScopedPointer<ParameterSlider> __osc1, __osc2, __osc3, __osc4, __masterMixer;
	Label __osc1Label, __osc2Label, __osc3Label, __osc4Label, __masterLabel;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MixerComponent)
};

#endif MIXER_COMPONENT_H