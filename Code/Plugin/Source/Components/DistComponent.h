#ifndef DIST_COMPONENT_H
#define DIST_COMPONENT_H

#include "Global.h"
#include "ParameterHandler.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "InformationComponent.h"
#include "ParameterSlider.h"
#include "ParameterButton.h"
#include "IVSTParameters.h"

class DistComponent : public Component, private IVSTParameters
{
public:
	//==============================================================================
	DistComponent(int ID,GLOBAL*global);
	~DistComponent();

	//==============================================================================

	void paint(Graphics& g) override;
	void resized() override;
	GLOBAL * Global;
private:
	//==============================================================================
	ScopedPointer<ParameterSlider> __threshold;
	//Label __thresholdLabel;
	ScopedPointer<ParameterButton> __toggleDist;
	Rectangle<int> __bounds;
	int __ID, __width, __height;
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DistComponent)
};
#endif