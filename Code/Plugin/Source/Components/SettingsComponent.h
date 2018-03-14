#ifndef SETTINGS_COMPONENT_H
#define SETTINGS_COMPONENT_H

#include "../Global.h"
#include "ParameterSlider.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "SelectKnob.h"

class SettingsComponent : public Component
{
public:
	SettingsComponent();
	~SettingsComponent();
	static void RegisterParameters(int ID);

private:
	void paint(Graphics& g) override;
	void resized() override;

	ScopedPointer<ParameterSlider> __pitchBendSens;
	ScopedPointer<SelectKnob> __selectKnob;
	Label __pitchBendLabel;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsComponent)
};

#endif SETTINGS_COMPONENT_H
