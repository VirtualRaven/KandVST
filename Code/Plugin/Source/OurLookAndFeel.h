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
		float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider);
private :

};

#endif