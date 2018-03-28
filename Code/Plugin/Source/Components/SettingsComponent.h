#ifndef SETTINGS_COMPONENT_H
#define SETTINGS_COMPONENT_H

#include "Global.h"
#include "ParameterSlider.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "SelectKnob.h"

class SettingsComponent : public Component
{
public:
	SettingsComponent(GLOBAL*global);
	~SettingsComponent();
	static void RegisterParameters(int ID, GLOBAL*Global);
	GLOBAL * Global;
private:
	void paint(Graphics& g) override;
	void resized() override;

	ScopedPointer<ParameterSlider> __pitchBendSens;
	ScopedPointer<SelectKnob> __selectKnob, __modChoice;
	Label __pitchBendLabel;
	Rectangle<int> __bounds;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsComponent)
};

#endif //SETTINGS_COMPONENT_H
