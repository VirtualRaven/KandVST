#ifndef MIXER_SUB_COMPONENT_H
#define MIXER_SUB_COMPONENT_H

#include "../Global.h"
#include "ParameterSlider.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "../IVSTParameters.h"
class MixerSubComponent : public Component,IVSTParameters
{
public:
	MixerSubComponent(int ID,String  label);
	MixerSubComponent(int ID, String  label, String overridePanId,String overrideGainId);
	~MixerSubComponent();

private: 
	void paint(Graphics& g) override;
	void resized() override;

	ScopedPointer<ParameterSlider> __gain;
	Label __label;

	ScopedPointer<ParameterSlider> __pan;
	Label __panL, __panR;
	Rectangle<int> __bounds;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MixerSubComponent)
};

#endif MIXER_SUB_COMPONENT_H