
#ifndef SELCT_KNOB_H
#define SELCT_KNOB_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterSlider.h"
class SelectKnobSlider: public ParameterSlider
{
public:
	SelectKnobSlider(AudioParameterChoice& p, float angleBetweenPos);
	~SelectKnobSlider();
	void valueChanged() override;
	void resized() override;

	void startedDragging() override;
	void stoppedDragging() override;

private:
	AudioParameterChoice& __param;
	float __angleBetweenPos;
};

class SelectKnob : public Component
{
public:
	SelectKnob(AudioParameterChoice& p);
	~SelectKnob();

private:
	float __angleBetweenPos;
	SelectKnobSlider __slider;
	std::vector<Label*> __labels;
	std::vector<float> __snapAngles;
	AudioParameterChoice& __param;
	Point<float> angleToPos(float angle, float d);


	void paint(Graphics& g) override;
	void resized() override;
};


#endif
