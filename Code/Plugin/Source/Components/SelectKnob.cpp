#include "SelectKnob.h"

SelectKnob::SelectKnob(AudioParameterChoice& p) :
	__param(p),
	__angleBetweenPos((float_Pi / 4.0f) * (-0.2f * p.choices.size() + 1.9f)), // *0.9 for esthetic purposes
	__snapAngles(),
	__slider(p, __angleBetweenPos)
{
	// The knob needs tweaking to work with more parameters
	jassert(__param.choices.size() <= 5);

	// Create snap positions (angles):
	float startAngle = -((__param.choices.size() - 1) * __angleBetweenPos / 2.0f);
	for (int i = 0; i < __param.choices.size(); i++)
	{
		float currentAngle = startAngle + i * __angleBetweenPos;
		__snapAngles.push_back(currentAngle);
	}

	addAndMakeVisible(__slider);
}

SelectKnob::~SelectKnob()
{

}

void SelectKnob::paint(Graphics& g)
{
	//g.setColour(Colour::fromRGB(255, 40, 40));
	//g.drawRect(0, 0, 120, 100);
	
	Font font(10, Font::FontStyleFlags::plain);
	g.setFont(font);

	// Draw dots and text
	for (int i = 0; i < __snapAngles.size(); i++)
	{
		float currentAngle = __snapAngles.at(i);

		// Draw dots
		Point<float> p = angleToPos(currentAngle, 27);
		g.setColour(Colour::fromRGB(120, 120, 120));
		g.drawEllipse(p.getX(), p.getY(), 1, 1, 2);

		Point<float> textP = angleToPos(currentAngle, 33);

		// X-offset based on angle
		textP.setX(textP.getX() + currentAngle * 6);

		// Y-offset based on angle
		int mul = -1;
		//if (currentAngle > 0) mul = 1;
		textP.setY(textP.getY() + mul * currentAngle * 1);

		// Draw text
		g.setColour(Colour::fromRGB(255, 255, 255));
		g.drawSingleLineText(__param.choices[i], textP.getX(), textP.getY(), Justification::horizontallyCentred);
	}
}

Point<float> SelectKnob::angleToPos(float angle, float r)
{
	// Angle from the knob's Y-axis to a (x, y) position where
	// the middle of the knob is the origin and r the distance from the knob's origin.
	// The position uses the top-left corner of the component as reference
	int sliderR = __slider.getWidth() / 2;
	int sliderX = __slider.getPosition().getX();
	int sliderY = __slider.getPosition().getY();

	return Point<float>(sliderX + sliderR + (r * sin(angle)),
						sliderY + sliderR - (r * cos(angle)));
}

void SelectKnob::resized()
{
	Rectangle<int> b = (getLocalBounds());
	int size = jmin<int>(b.getWidth()-60, b.getHeight()-40);
	int paddW = jmax<int>(0, b.getWidth() - size)/2;
	int paddh = jmax<int>(0, b.getHeight() - size) / 2;

	__slider.setBounds(paddW, paddh,size,size);
}

SelectKnobSlider::SelectKnobSlider(AudioParameterChoice& p, float angleBetweenPos) :
	__angleBetweenPos(angleBetweenPos),
	ParameterSlider(p),
	__param(p)
{
	updateSliderPos();
	setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	setMouseDragSensitivity(getMouseDragSensitivity()/2);
	
	// Set the knob's limits according to __angleBetweenPos
	float halfMaxAngle = (__angleBetweenPos * (__param.choices.size() - 1)) / 2.0f;
	setRotaryParameters((2.0f*float_Pi) - halfMaxAngle, (2.0f*float_Pi) + halfMaxAngle, true);
}


SelectKnobSlider::~SelectKnobSlider()
{
	
}

void SelectKnobSlider::valueChanged()
{ 
	param.setValueNotifyingHost((float)Slider::getValue()); 
}

void SelectKnobSlider::startedDragging()
{ 
	ParameterSlider::startedDragging();
}
void SelectKnobSlider::stoppedDragging()
{ 
	ParameterSlider::stoppedDragging();
}

void SelectKnobSlider::resized()
{
	ParameterSlider::resized();
}


