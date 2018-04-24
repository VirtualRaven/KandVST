#include "OurLookAndFeel.h"

OurLookAndFeel::~OurLookAndFeel() {
}

OurLookAndFeel::OurLookAndFeel(GLOBAL * global)
{
	Global = global;
	setColour(Slider::backgroundColourId, Colour::fromRGB(30, 30, 30));
	setColour(Slider::thumbColourId, Colour::fromRGB(26, 105, 180));
	setColour(Slider::trackColourId, Colour::fromRGB(26, 26, 26));
	setColour(ListBox::backgroundColourId, Colour::fromRGB(25, 45, 75));
	setColour(TextButton::textColourOnId, Swatch::white);
	setColour(TextButton::textColourOffId, Swatch::disabled);

}


void OurLookAndFeel::drawRotarySlider(Graphics& g, int x, int y, int width, int height
	, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider & slider)
{

	__themeColour = __themePicker.getColour(Global->paramHandler->Get<AudioParameterChoice>(-1, "THEME")->getIndex());

	bool isChoice = false;
	if (auto ps = dynamic_cast<ParameterSlider*>(&slider))
	{
		if (dynamic_cast<AudioParameterChoice*>(&(ps->param)))
		{
			// Move the slider down to the right to fit dot labels
			isChoice = true;
			width -= 20;
			height -= 20;
			y += 20;
			//x += 20;
		}
	}

	const float radius = jmin(width / 2, height / 2) - 4.0f;
	const float centreX = x + (width * 0.5f);
	const float centreY = y + (height * 0.5f);
	const float rx = centreX - radius;
	const float ry = centreY - radius;
	const float rw = radius * 2.0f;
	const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

	Image knobRef = ImageFileFormat::loadFrom(Resources::Icons::knobs_png, sizeof(Resources::Icons::knobs_png));
	int size = jmin<int>(height, width)-12;

	x = jmax<int>(x, (width - size) / 2);
	y = jmax<int>(y, (height - size) / 2);


	g.setOpacity(1.0f);
	if (!slider.isEnabled()) {
		g.setOpacity(0.5f);
	}
	
	if (isChoice) // Magic numbers :S
		g.drawImage(knobRef, Rectangle<float>(static_cast<float>(x), static_cast<float>(y+7), static_cast<float>(size), static_cast<float>(size)), RectanglePlacement::stretchToFit, false);
	else
		g.drawImage(knobRef, Rectangle<float>(static_cast<float>(x), static_cast<float>(y), static_cast<float>(size), static_cast<float>(size)), RectanglePlacement::stretchToFit, false);
	
	// g.drawEllipse(centreX, centreY, 1, 1, 1);

	Path p;
	const float pointerLength = radius * 0.2f;
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
			
			
			g.setColour(__themeColour);

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
		if (auto choice = dynamic_cast<AudioParameterChoice*>(&(ps->param))) {
			// AudioParameterChoice

			g.setColour(Colour(255, 100, 100));
			//g.drawRect(ps->getLocalBounds());

			Path dots;
			Path blueDot;

			float angDelta = (float_Pi / 4.0f) * (-0.2f * choice->choices.size() + 1.9f);

			// The knob's limits are set in ParameterSlider.cpp
			
			for (int i = 0; i <= choice->choices.size(); i++)
			{
				dots.addEllipse(centreX - 2, 20, 4, 4);
				dots.applyTransform(AffineTransform::rotation(angDelta*(i>0), centreX, centreY));
			}

			dots.applyTransform(AffineTransform::rotation(rotaryStartAngle - angDelta, centreX, centreY));
			Colour dotColour = Swatch::background.darker(0.2);
			Path dark = Path(dots);
			Path light = Path(dots);

			dark.applyTransform(AffineTransform::translation(-1.0f, -1.0f));
			g.setColour(dotColour.darker().withAlpha(0.4f));
			g.fillPath(dark);

			light.applyTransform(AffineTransform::translation(1.0f, 1.0f));
			g.setColour(dotColour.brighter().withAlpha(0.4f));
			g.fillPath(light);

			g.setColour(dotColour);
			g.fillPath(dots);

			// Draw text
			Font font(10, Font::FontStyleFlags::plain);
			g.setFont(font);
			g.setColour(Colour::fromRGB(255, 255, 255));

			float currentAngle = rotaryEndAngle - 2*float_Pi;

			// Loop over all dots from right to left
			for (int i = 1; i <= choice->choices.size(); i++)
			{
				Point<float> textP = dots.getPointAlongPath((dots.getLength() / static_cast<float>(choice->choices.size())) * static_cast<float>(i));
				
				textP.setX(textP.getX() + currentAngle * 8);
				textP.setY(textP.getY() - 10 + abs(currentAngle) * 2);
				
				//choice->choices[choice->choices.size() - i]
				g.drawSingleLineText(choice->choices[choice->choices.size() - i], textP.getX(), textP.getY(), Justification::horizontallyCentred);
				currentAngle -= angDelta;
			}

			// Draw dot progress
			g.setColour(__themeColour);
			int i = choice->getIndex();
			blueDot.addEllipse(centreX - 2, 20, 4, 4);
			blueDot.applyTransform(AffineTransform::rotation(i*angDelta + rotaryStartAngle, centreX, centreY));
			g.fillPath(blueDot);
			blueDot.clear();

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

			g.setColour(__themeColour);
			g.strokePath(pFill, pst);
		}
		Image waveImage;
		switch (ps->getWaveType()) {
			case 1: // sine
				waveImage = ImageFileFormat::loadFrom(Resources::Icons::sine_white_png, sizeof(Resources::Icons::sine_white_png));
				g.drawImageWithin(waveImage, x + 4, y + 12, width*0.5, height*0.35, RectanglePlacement::centred, false);
				break;
			case 2: // square
				waveImage = ImageFileFormat::loadFrom(Resources::Icons::square_white_png, sizeof(Resources::Icons::square_white_png));
				g.drawImageWithin(waveImage, x + 4, y + 14, width*0.5, height*0.3, RectanglePlacement::centred, false);
				break;
			case 3: // saw
				waveImage = ImageFileFormat::loadFrom(Resources::Icons::saw_white_png, sizeof(Resources::Icons::saw_white_png));
				g.drawImageWithin(waveImage, x, y + 14, width*0.6, height*0.3, RectanglePlacement::centred, false);
				break;
			case 4: // triangle
				waveImage = ImageFileFormat::loadFrom(Resources::Icons::triangle_white_png, sizeof(Resources::Icons::triangle_white_png));
				g.drawImageWithin(waveImage, x, y + 12, width*0.6, height*0.35, RectanglePlacement::centred, false);
				break;
			case 5: // noise
				waveImage = ImageFileFormat::loadFrom(Resources::Icons::noise_white_png, sizeof(Resources::Icons::noise_white_png));
				g.drawImageWithin(waveImage, x + 2, y + 10, width*0.55, height*0.4, RectanglePlacement::centred, false);
				break;
			default: 
				break;
		}
	}

	

	g.setColour(Colours::white);
	g.fillPath(p);

}

void OurLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height,
	float sliderPos,
	float minSliderPos,
	float maxSliderPos,
	Slider::SliderStyle style, Slider& slider)
{
	__themeColour = __themePicker.getColour(Global->paramHandler->Get<AudioParameterChoice>(-1, "THEME")->getIndex());
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
			slider.isHorizontal() ? height : y);

		//Draw lines
		const size_t numLines = 7;
		const size_t longEvery = 3;
		double lineStep = (static_cast<double>((height)) / static_cast<double>((numLines-1)));
		for (size_t i = 0; i < numLines; i++)
		{
			if (i % longEvery == 0)
			{
				//Draw long
				Path line;
				line.startNewSubPath(startPoint.x + 10.0f, round(i*lineStep + y - (trackWidth / 12.0f)));
				line.lineTo(startPoint.x + 10.0f + 20.0f, round(i*lineStep + y - (trackWidth / 12.0f)));
				line.startNewSubPath(startPoint.x - 10.0f, round(i*lineStep + y - (trackWidth / 12.0f)));
				line.lineTo(startPoint.x - 10.0f - 20.0f, round(i*lineStep + y - (trackWidth / 12.0f)));
				g.setColour(Swatch::background.brighter());

				g.strokePath(line, { trackWidth / 6.0f, PathStrokeType::curved, PathStrokeType::rounded });
			}
			else
			{
				//Draw short
				Path line;
				line.startNewSubPath(startPoint.x + 10.0f, round(i*lineStep + y - (trackWidth / 12.0f)));
				line.lineTo(startPoint.x + 10.0f + 10.0f, round(i*lineStep + y - (trackWidth / 12.0f)));
				line.startNewSubPath(startPoint.x - 10.0f, round(i*lineStep + y - (trackWidth / 12.0f)));
				line.lineTo(startPoint.x - 10.0f - 10.0f, round(i*lineStep + y - (trackWidth / 12.0f)));
				g.setColour(Swatch::background.brighter());

				g.strokePath(line, { trackWidth / 6.0f, PathStrokeType::curved, PathStrokeType::rounded });
			}
		}

		



		Path backgroundTrack;
		backgroundTrack.startNewSubPath(startPoint);
		backgroundTrack.lineTo(endPoint);
		

		Path backgroundTrack_light(backgroundTrack);
		Path backgroundTrack_dark(backgroundTrack);
		backgroundTrack_dark.applyTransform(AffineTransform::translation(-1, -1));
		backgroundTrack_light.applyTransform(AffineTransform::translation(1, 1));

		Colour dotColour = Swatch::background.darker(0.2);

		g.setColour(dotColour.darker().withAlpha(0.2f));

		g.strokePath(backgroundTrack_dark, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });
		g.setColour(dotColour.brighter().withAlpha(0.2f));

		g.strokePath(backgroundTrack_light, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });
		g.setColour(dotColour);

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
		g.setColour(__themeColour);
		g.strokePath(valueTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });

		if (!isTwoVal)
		{

			// CHANGE SLIDER THUMB HERE

			Image verticalSlider = ImageFileFormat::loadFrom(Resources::Icons::sliderthumb_png, sizeof(Resources::Icons::sliderthumb_png));
			
			g.setOpacity(1.0f);
			if (!slider.isEnabled()) {
				g.setOpacity(0.5f);
			}
			g.drawImage(verticalSlider, Rectangle<float>(static_cast<float>(thumbWidth * 3.5), static_cast<float>(thumbWidth * 3.5)).withCentre(isThreeVal ? thumbPoint : maxPoint), RectanglePlacement::centred, false);

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
	__themeColour = __themePicker.getColour(Global->paramHandler->Get<AudioParameterChoice>(-1, "THEME")->getIndex());
	//setColour(TextButton::ColourIds::buttonOnColourId, __themeColour.darker(0.7f).withSaturation(0.5f));
		
	auto baseColour = backgroundColour.withMultipliedSaturation(button.hasKeyboardFocus(true) ? 1.0f : 1.0f)
		.withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f);

	if (isButtonDown || isMouseOverButton)
		baseColour = baseColour.contrasting(isButtonDown ? 0.2f : 0.05f);
	
	if (button.getToggleState())
		baseColour = __themeColour.darker(0.7f).withSaturation(0.5f);

	g.setColour(baseColour);

	Path path;
	auto bounds = button.getLocalBounds().toFloat().reduced(0.5f, 0.5f);
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


