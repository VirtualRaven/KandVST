#include "OurLookAndFeel.h"

OurLookAndFeel::~OurLookAndFeel() {
}

OurLookAndFeel::OurLookAndFeel() {
	
	setColour(Slider::backgroundColourId, Colour::fromRGB(30,30,30));
	setColour(Slider::thumbColourId, Colour::fromRGB(26, 105, 180));
	setColour(Slider::trackColourId, Colour::fromRGB(26, 26, 26));
}


void OurLookAndFeel::drawRotarySlider(Graphics& g, int x, int y, int width, int height
	, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider & /*slider*/)
{
	const float radius = jmin(width / 2, height / 2) - 4.0f;
	const float centreX = x + width * 0.5f;
	const float centreY = y + height * 0.5f;
	const float rx = centreX - radius;
	const float ry = centreY - radius;
	const float rw = radius * 2.0f;
	const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

	Image knobRef = ImageFileFormat::loadFrom(Resources::Icons::knobs_png, sizeof(Resources::Icons::knobs_png));
	g.setOpacity(1.0f);
	g.drawImage(knobRef, Rectangle<float>(static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height)), RectanglePlacement::stretchToFit, false);

	Path p;
	const float pointerLength = radius * 0.3f;
	const float pointerThickness = 5.0f;
	p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
	p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));

	g.setColour(Colours::white);
	g.fillPath(p);

}

void OurLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height,
	float sliderPos,
	float minSliderPos,
	float maxSliderPos,
	Slider::SliderStyle style, Slider& slider)
{
	if (slider.isBar())
	{
		g.setColour(slider.findColour(Slider::trackColourId));
		g.fillRect(slider.isHorizontal() ? Rectangle<float>(static_cast<float> (x), y + 0.5f, sliderPos - x, height - 1.0f)
			: Rectangle<float>(x + 0.5f, sliderPos, width - 1.0f, y + (height - sliderPos)));
	}
	else
	{
		auto isTwoVal = (style == Slider::SliderStyle::TwoValueVertical || style == Slider::SliderStyle::TwoValueHorizontal);
		auto isThreeVal = (style == Slider::SliderStyle::ThreeValueVertical || style == Slider::SliderStyle::ThreeValueHorizontal);

		auto trackWidth = jmin(6.0f, slider.isHorizontal() ? height * 0.25f : width * 0.25f);

		Point<float> startPoint(slider.isHorizontal() ? x : x + width * 0.5f,
			slider.isHorizontal() ? y + height * 0.5f : height + y);

		Point<float> endPoint(slider.isHorizontal() ? width + x : startPoint.x,
			slider.isHorizontal() ? startPoint.y : y);

		Path backgroundTrack;
		backgroundTrack.startNewSubPath(startPoint);
		backgroundTrack.lineTo(endPoint);
		g.setColour(slider.findColour(Slider::backgroundColourId));
		g.strokePath(backgroundTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });

		Path valueTrack;
		Point<float> minPoint, maxPoint, thumbPoint;

		if (isTwoVal || isThreeVal)
		{
			minPoint = { slider.isHorizontal() ? minSliderPos : width * 0.5f,
				slider.isHorizontal() ? height * 0.5f : minSliderPos };

			if (isThreeVal)
				thumbPoint = { slider.isHorizontal() ? sliderPos : width * 0.5f,
				slider.isHorizontal() ? height * 0.5f : sliderPos };

			maxPoint = { slider.isHorizontal() ? maxSliderPos : width * 0.5f,
				slider.isHorizontal() ? height * 0.5f : maxSliderPos };
		}
		else
		{
			auto kx = slider.isHorizontal() ? sliderPos : (x + width * 0.5f);
			auto ky = slider.isHorizontal() ? (y + height * 0.5f) : sliderPos;

			minPoint = startPoint;
			maxPoint = { kx, ky };
		}

		auto thumbWidth = getSliderThumbRadius(slider);

		valueTrack.startNewSubPath(minPoint);
		valueTrack.lineTo(isThreeVal ? thumbPoint : maxPoint);
		g.setColour(slider.findColour(Slider::trackColourId));
		g.strokePath(valueTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });

		if (!isTwoVal)
		{

			// CHANGE SLIDER THUMB HERE

			/*
			g.setColour(slider.findColour(Slider::thumbColourId));
			g.fillEllipse(Rectangle<float>(static_cast<float> (thumbWidth), static_cast<float> (thumbWidth)).withCentre(isThreeVal ? thumbPoint : maxPoint));
			*/

			Image verticalSlider = ImageFileFormat::loadFrom(Resources::Icons::sliderthumb_png, sizeof(Resources::Icons::sliderthumb_png));
			g.setOpacity(1.0f);
			g.drawImage(verticalSlider, Rectangle<float>(static_cast<float>(thumbWidth * 5), static_cast<float>(thumbWidth * 5)).withCentre(isThreeVal ? thumbPoint : maxPoint), RectanglePlacement::centred, false);

		}

		if (isTwoVal || isThreeVal)
		{
			auto sr = jmin(trackWidth, (slider.isHorizontal() ? height : width) * 0.4f);
			auto pointerColour = Colours::beige;


			if (slider.isHorizontal())
			{
				drawPointer(g, minSliderPos - sr,
					jmax(0.0f, y + height * 0.5f - trackWidth * 2.0f),
					trackWidth * 2.0f, pointerColour, 2);

				drawPointer(g, maxSliderPos - trackWidth,
					jmin(y + height - trackWidth * 2.0f, y + height * 0.5f),
					trackWidth * 2.0f, pointerColour, 4);
			}
			else
			{
				drawPointer(g, jmax(0.0f, x + width * 0.5f - trackWidth * 2.0f),
					minSliderPos - trackWidth,
					trackWidth * 2.0f, pointerColour, 1);

				drawPointer(g, jmin(x + width - trackWidth * 2.0f, x + width * 0.5f), maxSliderPos - sr,
					trackWidth * 2.0f, pointerColour, 3);
			}
		}
	}
}

