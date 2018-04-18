#ifndef LFO_COMPONENT_H
#define LFO_COMPONENT_H

#include "Global.h"
#include "ParameterHandler.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterSlider.h"
#include "ParameterButton.h"
#include "IVSTParameters.h"
#include "Swatch.h"

class LFOComponent : public Component, private IVSTParameters, public ComboBox::Listener
{
public:
    //==============================================================================
	LFOComponent(int ID,GLOBAL*global);
    ~LFOComponent();

    //==============================================================================

    void paint (Graphics& g) override;
    void resized() override;
	GLOBAL * Global;
	virtual void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
private:
	//==============================================================================
	ScopedPointer<ParameterSlider> __type, __amp;
	Label __ratioLabel, __typeLabel, __ampLabel;
	ScopedPointer<ParameterButton> __toggleLfo, __toggleInvert, __onPress;
	Rectangle<int> __bounds;
	int __ID, __width, __height;
	ComboBox __cBox;
	StringArray __choices;
	AudioParameterChoice * __choiceParameter;
	//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOComponent)
};
#endif