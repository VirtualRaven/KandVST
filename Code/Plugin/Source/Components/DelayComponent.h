#ifndef DELAY_COMPONENT_H
#define DELAY_COMPONENT_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "../IVSTParameters.h"
#include "ParameterSlider.h"
#include "../Global.h"

class DelayComponent : public Component, private IVSTParameters {
public:
	DelayComponent();
	~DelayComponent();
	//========
	void paint(Graphics& g) override;
	void resized() override;


private:
	ScopedPointer<ParameterSlider> __delaySpeed, __delaySeconds;
	Label __speedLabel, __secondsLabel;
	Rectangle<int> __bounds;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayComponent);
};

#endif
