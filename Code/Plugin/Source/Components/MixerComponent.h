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

	ScopedPointer<ParameterSlider> __masterPan, __pan1, __pan2, __pan3, __pan4;
	Label __panL, __panR,  __pan1L, __pan1R, __pan2L, __pan2R, __pan3L, __pan3R, __pan4L, __pan4R;
	Rectangle<int> __bounds;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MixerComponent)
};

#endif MIXER_COMPONENT_H