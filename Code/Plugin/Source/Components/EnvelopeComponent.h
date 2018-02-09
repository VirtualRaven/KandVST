#ifndef ENVELOPE_COMPONENT_H
#define ENVELOPE_COMPONENT_H
#include "../Global.h"
#include "../ParameterHandler.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterSlider.h"
#include "../IVSTParameters.h"
class EnvelopeComponent: public  Component, private IVSTParameters
{
private:
	Label attackLabel, decayLabel, sustainLabel, releaseLabel;
	ScopedPointer<ParameterSlider> attack, decay, release, sustain;
public:
	EnvelopeComponent(int ID);
	~EnvelopeComponent();
	void resized() override;
};

#endif