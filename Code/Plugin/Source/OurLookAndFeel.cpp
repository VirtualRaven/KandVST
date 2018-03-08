#include "OurLookAndFeel.h"

OurLookAndFeel::~OurLookAndFeel() {
}

OurLookAndFeel::OurLookAndFeel() {
	
	setColour(Slider::backgroundColourId, Colour::fromRGB(30,30,30));
	setColour(Slider::thumbColourId, Colour::fromRGB(26, 105, 180));
	LookAndFeel_V4::setColour(Slider::trackColourId, Colour::fromRGB(26, 26, 26));
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

		auto thumbWidth = LookAndFeel_V4::getSliderThumbRadius(slider);

		valueTrack.startNewSubPath(minPoint);
		valueTrack.lineTo(isThreeVal ? thumbPoint : maxPoint);
		g.setColour(slider.findColour(Slider::trackColourId));
		g.strokePath(valueTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });

		if (!isTwoVal)
		{

			// CHANGE SLIDER THUMB HERE

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



void OurLookAndFeel::drawTabButton(TabBarButton & button, Graphics & g, bool isMouseOver, bool isMouseDown)
{
	Rectangle<int> activeArea(button.getActiveArea());

	const TabbedButtonBar::Orientation o = button.getTabbedButtonBar().getOrientation();

	Colour bkg(button.getTabBackgroundColour());

	Colour textCol;
	Image tab;
	if (button.getToggleState())
	{
		if (bkg == Colour::fromRGB(200, 200, 200)) { //master tab
			tab = ImageFileFormat::loadFrom(Resources::Icons::tb_active_master_png, sizeof(Resources::Icons::tb_active_master_png));
		}
		else {	//oscillator tab
			tab = ImageFileFormat::loadFrom(Resources::Icons::tb_active_png, sizeof(Resources::Icons::tb_active_png));
		}
		textCol = textCol.fromRGB(36, 36, 36);
	}
	else
	{
		tab = ImageFileFormat::loadFrom(Resources::Icons::tb_inactive_png, sizeof(Resources::Icons::tb_inactive_png));
		textCol = Colours::white;
	}

	g.drawImage(tab, activeArea.toFloat(), RectanglePlacement::stretchToFit, false);
	
	const Rectangle<float> area(button.getTextArea().toFloat());

	float length = area.getWidth();
	float depth = area.getHeight();

	if (button.getTabbedButtonBar().isVertical())
		std::swap(length, depth);

	TextLayout textLayout;
	createTabTextLayout(button, length, depth, textCol, textLayout);
	textLayout.draw(g, Rectangle<float>(length, depth));
}


void OurLookAndFeel::drawButtonText(Graphics & g, TextButton & button, bool isMouseOverButton, bool isButtonDown)
{
	Font font(getTextButtonFont(button, button.getHeight()));
	font.setStyleFlags(Font::FontStyleFlags::bold);
	g.setFont(font);
	g.setColour(button.findColour(button.getToggleState() ? TextButton::textColourOnId
		: TextButton::textColourOffId)
		.withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f));

	const int yIndent = 0;//jmin(4, button.proportionOfHeight(0.3f));
	const int cornerSize = 0;

	const int fontHeight = roundToInt(font.getHeight() * 0.6f);
	const int leftIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
	const int rightIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
	const int textWidth = button.getWidth() - leftIndent - rightIndent;

	if (textWidth > 0)
		g.drawFittedText(button.getButtonText(),
			leftIndent, yIndent, textWidth, button.getHeight(),//- yIndent * 2,
			Justification::centred, 0);
}

void OurLookAndFeel::createTabTextLayout(const TabBarButton & button, float length, float depth, Colour colour, TextLayout & textLayout)
{
	Font font(depth * 0.5f, Font::FontStyleFlags::bold);
	font.setUnderline(button.hasKeyboardFocus(false));

	AttributedString s;
	s.setJustification(Justification::centred);
	s.append(button.getButtonText().trim(), font, colour);

	textLayout.createLayout(s, length);
}


