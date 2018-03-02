#ifndef OUR_LOOK_AND_FEEL_H
#define OUR_LOOK_AND_FEEL_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "Resources_files.h"

class OurLookAndFeel : public LookAndFeel_V4 
{
public:
	 OurLookAndFeel();
	~OurLookAndFeel();

	void drawRotarySlider(Graphics& g, int x, int y, int width, int height,
		float sliderPos, float rotaryStartAngle,  float rotaryEndAngle, Slider& slider) override;
	
	void drawLinearSlider(Graphics& g, int x, int y, int width, int height,
		float sliderPos, float minSliderPos, float maxSliderPos, Slider::SliderStyle style, Slider& slider) override;

private :

};

#endif