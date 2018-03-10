
#ifndef SELCT_KNOB_H
#define SELCT_KNOB_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterSlider.h"
class SelectKnob: public ParameterSlider
{
public:
	SelectKnob(AudioParameterChoice& p);
	~SelectKnob();
	void valueChanged() override;
	void resized() override;
	void paint(Graphics& g) override; // Temp?

	void startedDragging() override;
	void stoppedDragging() override;

private:
	std::vector<Label*> __labels;
	AudioParameterChoice& __param;
};


#endif
