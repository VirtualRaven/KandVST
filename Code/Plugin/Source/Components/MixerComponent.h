#ifndef MIXER_COMPONENT_H
#define MIXER_COMPONENT_H

#include "Global.h"
#include "ParameterSlider.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "MixerSubComponent.h"
class MixerComponent : public Component
{
public:
	MixerComponent(GLOBAL*global);
	~MixerComponent();

private: 
	void paint(Graphics& g) override;
	void resized() override;
	std::vector<MixerSubComponent*> __mixers;
	Rectangle<int> __bounds;
	GLOBAL * Global;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MixerComponent)
};

#endif //MIXER_COMPONENT_H
