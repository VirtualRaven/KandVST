
#ifndef SELCT_KNOB_H
#define SELCT_KNOB_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterSlider.h"
class SelectKnobSlider: public ParameterSlider
{
public:
	SelectKnobSlider(AudioParameterChoice& p, float angleBetweenPos,GLOBAL*global);
	~SelectKnobSlider();
	void valueChanged() override;
	void resized() override;
	GLOBAL*Global;
private:
	AudioParameterChoice& __param;
	float __angleBetweenPos;
	void paint(Graphics& g) override;
};

class SelectKnob : public Component
{
public:
	SelectKnob(AudioParameterChoice& p,GLOBAL*global);
	~SelectKnob();
	GLOBAL*Global;
private:
	float __angleBetweenPos;
	SelectKnobSlider __slider;
	std::vector<float> __snapAngles;
	AudioParameterChoice& __param;
	Point<float> angleToPos(float angle, float d);


	void paint(Graphics& g) override;
	void resized() override;
};


#endif
