
#ifndef SELCT_KNOB_H
#define SELCT_KNOB_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterSlider.h"
class SelectKnobSlider: public ParameterSlider
{
public:
	SelectKnobSlider(AudioParameterChoice& p);
	~SelectKnobSlider();
	void valueChanged() override;
	void resized() override;

	void startedDragging() override;
	void stoppedDragging() override;

private:
	AudioParameterChoice& __param;
};

class SelectKnob : public Component
{
public:
	SelectKnob(AudioParameterChoice& p);
	~SelectKnob();

private:
	SelectKnobSlider __slider;
	std::vector<Label*> __labels;
	AudioParameterChoice& __param;

	void paint(Graphics& g) override;
	void resized() override;
};


#endif
