#include "SelectKnob.h"

SelectKnob::SelectKnob(AudioParameterChoice& p) :
	__angleBetweenPos((float_Pi / 4.0f) * 0.9f),
	__param(p),
	__labels(),
	__snapAngles(),
	__slider(p, __angleBetweenPos)
{
	addAndMakeVisible(__slider);

	// Create snap positions:
	float startAngle = -((__param.choices.size() - 1) * __angleBetweenPos / 2.0f);
	for (int i = 0; i < __param.choices.size(); i++)
	{
		float currentAngle = startAngle + i * __angleBetweenPos;
		__snapAngles.push_back(currentAngle);
	}

	Font font(10, Font::FontStyleFlags::plain);

	for (int i = 0; i < __param.choices.size(); i++)
	{
		Label* l = new Label(__param.choices[i], __param.choices[i]);
		l->setFont(font);
		l->setText(__param.choices[i], NotificationType::sendNotification);
		__labels.push_back(l);
		addAndMakeVisible(*l);
	}
}

SelectKnob::~SelectKnob()
{
	for (int i = 0; i < __labels.size(); i++)
	{
		delete __labels.at(i);
	}
}

void SelectKnob::paint(Graphics& g)
{
	g.setColour(Colour::fromRGB(255, 40, 40));
	g.drawRect(0, 0, 100, 100);
	g.drawRect(getBounds());
	
	// Draw dots
	for (auto angle : __snapAngles)
	{
		Point<float> p = angleToPos(angle, 27);
		g.drawEllipse(p.getX(), p.getY(), 1, 1, 2);
	}
}

Point<float> SelectKnob::angleToPos(float angle, float r)
{
	int sliderR = __slider.getWidth() / 2;
	int sliderX = __slider.getPosition().getX();
	int sliderY = __slider.getPosition().getY();

	return Point<float>(sliderX + sliderR + (r * sin(angle)),
						sliderY + sliderR - (r * cos(angle)));
}

void SelectKnob::resized()
{
	setSize(100, 100);
	Rectangle<int> box(getLocalBounds());
	__slider.setBounds(box);


	for (int i = 0; i < __labels.size(); i++)
	{
		float angle = __snapAngles.at(i);
		Point<float> p = angleToPos(angle, 30);
		__labels.at(i)->setBounds(getLocalBounds());

		__labels.at(i)->setCentrePosition(static_cast<int>(__slider.getX() + p.getX()), static_cast<int>(p.getY()));
	}
}

SelectKnobSlider::SelectKnobSlider(AudioParameterChoice& p, float angleBetweenPos) :
	__angleBetweenPos(angleBetweenPos),
	ParameterSlider(p),
	__param(p)
{
	updateSliderPos();
	setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	
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
	param.beginChangeGesture(); 
}
void SelectKnobSlider::stoppedDragging()
{ 
	param.endChangeGesture(); 
}

void SelectKnobSlider::resized()
{
	setBounds(30, 20, 50, 50);
	ParameterSlider::resized();
}


