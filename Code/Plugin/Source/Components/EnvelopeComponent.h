#ifndef ENVELOPE_COMPONENT_H
#define ENVELOPE_COMPONENT_H

#include "Global.h"
#include "ParameterHandler.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterSlider.h"
#include "IVSTParameters.h"
#include "OurLookAndFeel.h"
#include "EnvelopeGenerator.h"
#include "ParameterListener.h"
class EnvelopeComponent : public  Component, private IVSTParameters, private Timer, private ParameterListener,private ComponentListener {
private:
	ImageComponent __envImageComponent;
	Image *		   __envImage;
	Label attackLabel, decayLabel, sustainLabel, releaseLabel, holdLabel;
	ScopedPointer<ParameterSlider> attackTime, holdTime, decayTime, releaseTime, sustainTime,
								   attackCurve, decayCurve, releaseCurve, sustainCurve,
								   attackLevel, decayLevel, sustainLevel;

	bool __envInvalid;
	virtual void timerCallback() override;
	virtual void componentVisibilityChanged(Component &component) override;
	virtual void componentParentHierarchyChanged(Component &component) override;
	GLOBAL * Global;
	bool __linked = false;
	//void Link(int id);
	//void Unlink();
	//virtual void mouseUp(const MouseEvent& event) override;

public:
	EnvelopeComponent(int ID,GLOBAL*global);
	~EnvelopeComponent();
	void resized() override;
	void paint(Graphics & g) override;
	virtual void lookAndFeelChanged() override;

	// Inherited via ParameterListener
	virtual void parametersChanged(std::vector<std::string>) override;
};
#endif
