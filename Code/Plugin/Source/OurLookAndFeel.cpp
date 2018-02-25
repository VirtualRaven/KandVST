#include "OurLookAndFeel.h"


OurLookAndFeel::~OurLookAndFeel() {
}

OurLookAndFeel::OurLookAndFeel() {
	setColour(Slider::backgroundColourId, Colour::fromRGB(30,30,30));
	setColour(Slider::thumbColourId, Colour::fromRGB(26, 105, 180));
	setColour(Slider::trackColourId, Colour::fromRGB(26, 26, 26));
}


void OurLookAndFeel::drawRotarySlider(Graphics& g, int x, int y, int width, int height
	, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider & slider)
{
	const float radius = jmin(width / 2, height / 2) - 4.0f;
	const float centreX = x + width * 0.5f;
	const float centreY = y + height * 0.5f;
	const float rx = centreX - radius;
	const float ry = centreY - radius;
	const float rw = radius * 2.0f;
	const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

	Image& knobRef = ImageFileFormat::loadFrom(Resources::Icons::knobs_png, sizeof(Resources::Icons::knobs_png));
	g.setOpacity(1.0f);
	g.drawImage(knobRef, Rectangle<float>(x,y,width,height), RectanglePlacement::stretchToFit, false);

	Path p;
	const float pointerLength = radius * 0.3f;
	const float pointerThickness = 5.0f;
	p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
	p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));

	g.setColour(Colours::white);
	g.fillPath(p);

}
