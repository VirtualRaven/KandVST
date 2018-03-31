#ifndef REVERB_COMPONENT_H
#define REVERB_COMPONENT_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "Global.h"
#include "ConvolutionReverb.h"
#include "ParameterSlider.h"
#include "IVSTParameters.h"
#include "Swatch.h"

class ReverbComponent : public Component, public ComboBox::Listener {
public:
	ReverbComponent(int ID, GLOBAL*global);
	~ReverbComponent();

	void paint(Graphics& g) override;
	void resized() override;
	virtual void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;

private:
	ScopedPointer<ParameterSlider> __dry, __wet;
	ComboBox __cBox;
	int __ID;
	GLOBAL * Global;
};

#endif REVERB_COMPONENT_H
