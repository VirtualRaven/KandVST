#ifndef FILTER_COMPONENT_H
#define FILTER_COMPONENT_H

#include "../Global.h"
#include "ParameterSlider.h"
#include "../JuceLibraryCode/JuceHeader.h"

class FilterComponent : public Component
{
public:
	FilterComponent();
	~FilterComponent();

private:
	void paint(Graphics& g) override;
	void resized() override;

	ScopedPointer<ParameterSlider> __hpFilter, __lpFilter;
	Label __hpLabel, __lpLabel;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterComponent)
};

#endif FILTER_COMPONENT_H
