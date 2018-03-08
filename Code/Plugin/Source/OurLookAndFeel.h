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

	void drawTabButton(TabBarButton&, Graphics&, bool isMouseOverButton, bool isMouseDown) override;

	void drawButtonText(Graphics& g, TextButton& button, bool isMouseOverButton, bool isButtonDown) override;

	static void createTabTextLayout(const TabBarButton& button, float length, float depth, Colour colour, TextLayout&);

private :
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OurLookAndFeel)
};

#endif