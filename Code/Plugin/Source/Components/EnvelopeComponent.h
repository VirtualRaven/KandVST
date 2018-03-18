#ifndef ENVELOPE_COMPONENT_H
#define ENVELOPE_COMPONENT_H

#include "../Global.h"
#include "../ParameterHandler.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterSlider.h"
#include "../IVSTParameters.h"
#include "../OurLookAndFeel.h"
#include "../EnvelopeGenerator.h"
#include "../ParameterListener.h"
class EnvelopeComponent : public  Component, private IVSTParameters, private Timer, private ParameterListener
{
private:
	ImageComponent __envImageComponent;
	Image *		   __envImage;
	Label attackLabel, decayLabel, sustainLabel, releaseLabel, holdLabel;
	ScopedPointer<ParameterSlider> attackTime, holdTime, decayTime, releaseTime, sustainTime,
								   attackCurve, decayCurve, releaseCurve, sustainCurve,
								   attackLevel, decayLevel, sustainLevel;
	bool __envInvalid;
	virtual void timerCallback() override;

public:
	EnvelopeComponent(int ID);
	~EnvelopeComponent();
	void resized() override;
	void paint(Graphics & g) override;

	// Inherited via ParameterListener
	virtual void parametersChanged(std::vector<std::string>) override;
};
#endif
