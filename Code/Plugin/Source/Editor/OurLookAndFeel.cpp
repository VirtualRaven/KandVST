#include "OurLookAndFeel.h"

OurLookAndFeel::~OurLookAndFeel() {
}

OurLookAndFeel::OurLookAndFeel() {
	
	setColour(Slider::backgroundColourId, Colour::fromRGB(30,30,30));
	setColour(Slider::thumbColourId, Colour::fromRGB(26, 105, 180));
	setColour(Slider::trackColourId, Colour::fromRGB(26, 26, 26));
	setColour(ListBox::backgroundColourId, Colour::fromRGB(25, 45, 75));
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

	Image knobRef = ImageFileFormat::loadFrom(Resources::Icons::knobs_png, sizeof(Resources::Icons::knobs_png));
	int size = jmin<int>(height, width)-12;

	x = jmax<int>(x, (width - size)/2);
	y = jmax<int>(y, (height - size)/2);

	g.setOpacity(1.0f);
	if (!slider.isEnabled()) {
		g.setOpacity(0.5f);
	}
	g.drawImage(knobRef, Rectangle<float>(static_cast<float>(x), static_cast<float>(y), static_cast<float>(size), static_cast<float>(size)), RectanglePlacement::stretchToFit, false);

	Path p;
	const float pointerLength = radius * 0.3f;
	const float pointerThickness = pointerLength * 0.6f;
	p.addRectangle(-pointerThickness * 0.5f, -radius+4, pointerThickness, pointerLength);
	p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));

	

	//Only applies to parameterSlider
	if (auto ps = dynamic_cast<ParameterSlider*>(&slider))
	{
		if (auto intParam = dynamic_cast<AudioParameterInt*>(&(ps->param))) {
			
			
			Path dots;
			Path blueDot;

			float ang = 0.0f;
			Range<int> range = intParam->getRange();
			float angDelta = (rotaryStartAngle - rotaryEndAngle) / intParam->getRange().getLength();
			for (int i = 0; i <= intParam->getRange().getLength()+1; i++)
			{
				dots.addEllipse(centreX-2, 0, 4, 4);
				dots.applyTransform(AffineTransform::rotation(angDelta*(i>0), centreX, centreY));			
			}
			
			dots.applyTransform(AffineTransform::rotation(rotaryEndAngle- angDelta, centreX, centreY));
			Colour dotColour = Swatch::background.darker(0.2);
			Path dark = Path(dots);
			Path light = Path(dots);

			dark.applyTransform(AffineTransform::translation(-1.0f,-1.0f));
			g.setColour(dotColour.darker().withAlpha(0.4f));
			g.fillPath(dark);

			light.applyTransform(AffineTransform::translation(1.0f, 1.0f));
			g.setColour(dotColour.brighter().withAlpha(0.4f));
			g.fillPath(light);

			g.setColour(dotColour);
			g.fillPath(dots);

			g.setColour(Swatch::accentBlue);
			for (int i = 0; i <= intParam->getRange().getLength() + 1; i++)
			{
				switch (ps->getDrawProgress())
				{
				case ParameterSlider::ProgressStart::SingleDot:
					if ((i + range.getStart()) == (*intParam))
					{
						blueDot.addEllipse(centreX - 2, 0, 4, 4);
						blueDot.applyTransform(AffineTransform::rotation(-angDelta * i + rotaryStartAngle, centreX, centreY));
						g.fillPath(blueDot);
						blueDot.clear();
					}
					break;
				case ParameterSlider::ProgressStart::Start:

					if ((i + range.getStart()) <= (*intParam))
					{
						blueDot.addEllipse(centreX - 2, 0, 4, 4);
						blueDot.applyTransform(AffineTransform::rotation(-angDelta * i + rotaryStartAngle, centreX, centreY));
						g.fillPath(blueDot);
						blueDot.clear();
					}
					break;
				case ParameterSlider::ProgressStart::Center:

					if (((i + range.getStart()) >= (*intParam) && (i + range.getStart()) <= 0) || ((i + range.getStart()) <= (*intParam) && (i + range.getStart()) >= 0))
					{
						blueDot.addEllipse(centreX - 2, 0, 4, 4);
						blueDot.applyTransform(AffineTransform::rotation(-angDelta * i + rotaryStartAngle, centreX, centreY));

						g.fillPath(blueDot);
						blueDot.clear();

					}

					break;
				default:
					break;
				}
			}
			
		}
		else if (auto floatParam = dynamic_cast<AudioParameterFloat*>(&(ps->param)) && ps->getDrawProgress()!=ParameterSlider::ProgressStart::Disabled)
		{
			Colour dotColour = Swatch::background.darker(0.2);

			Path pFill;
			Path pL,pD,pB;
			switch (ps->getDrawProgress())
			{
			case ParameterSlider::ProgressStart::Start:
				pFill.addCentredArc(centreX, centreY, radius, radius, 0, rotaryStartAngle, angle, true);
				break;
			case ParameterSlider::ProgressStart::Center:
				pFill.addCentredArc(centreX, centreY, radius, radius, 0, 0, angle - 2 * 3.14f, true);
				break;
			case ParameterSlider::ProgressStart::End:
				pFill.addCentredArc(centreX, centreY, radius, radius, 0, rotaryEndAngle, angle, true);
				break;
			}
			
			pL.addCentredArc(centreX, centreY, radius, radius, 0, rotaryStartAngle, rotaryEndAngle, true);
			pD = Path(pL);
			pB = Path(pL);

			pL.applyTransform(AffineTransform::translation(1.0f, 1.0f));
			pD.applyTransform(AffineTransform::translation(-1.0f, -1.0f));

			PathStrokeType pst(4.0f);

			g.setColour(dotColour.darker().withAlpha(0.2f));
			g.strokePath(pD,pst);

			g.setColour(dotColour.brighter().withAlpha(0.2f));
			g.strokePath(pL, pst);

			g.setColour(dotColour);
			g.strokePath(pB, pst);

			g.setColour(Swatch::accentBlue);
			g.strokePath(pFill, pst);
		}
	}


	g.setColour(Colours::white);
	g.fillPath(p);
	//DEBUG

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
			if (!slider.isEnabled()) {
				g.setOpacity(0.5f);
			}
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

void OurLookAndFeel::drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown)
{
	auto bounds = button.getLocalBounds().toFloat().reduced(0.5f, 0.5f);

	auto baseColour = backgroundColour.withMultipliedSaturation(button.hasKeyboardFocus(true) ? 1.3f : 0.9f)
		.withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f);

	if (isButtonDown || isMouseOverButton)
		baseColour = baseColour.contrasting(isButtonDown ? 0.2f : 0.05f);

	g.setColour(baseColour);

	Path path;
	path.addRectangle(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
	g.fillPath(path);

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

void OurLookAndFeel::drawButtonText(Graphics & g, TextButton & button, bool isMouseOverButton, bool isButtonDown)
{
	Font font(button.getHeight() * 0.7, Font::bold);
	g.setFont(font);
	g.setColour(button.findColour(button.getToggleState() ? TextButton::textColourOnId
		: TextButton::textColourOffId)
		.withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f));

	const int yIndent = jmin(4, button.proportionOfHeight(0.3f));
	const int cornerSize = jmin(button.getHeight(), button.getWidth()) / 2;

	const int fontHeight = roundToInt(font.getHeight() * 0.6f);
	const int leftIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
	const int rightIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
	const int textWidth = button.getWidth() - leftIndent - rightIndent;

	if (textWidth > 0)
		g.drawFittedText(button.getButtonText(),
			leftIndent, yIndent, textWidth, button.getHeight() - yIndent * 2,
			Justification::centred, 2);
}

