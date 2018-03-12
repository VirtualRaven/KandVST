#include "SelectKnob.h"

SelectKnob::SelectKnob(AudioParameterChoice& p) :
	__slider(p),
	__param(p),
	__labels()
{
	addAndMakeVisible(__slider);

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
	g.drawRect(getBounds().reduced(5));
}

void SelectKnob::resized()
{
	Rectangle<int> box(getLocalBounds());
	__slider.setBounds(box);

	box = box.expanded(10);

	for (int i = 0; i < __labels.size(); i++)
	{
		__labels.at(i)->setBounds(box.removeFromTop(10));
	}
}

SelectKnobSlider::SelectKnobSlider(AudioParameterChoice& p) :
	ParameterSlider(p),
	__param(p)
{
	updateSliderPos();
	setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	
	setRotaryParameters(1.5*float_Pi, 2.5*float_Pi, true);
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
	setBounds(Rectangle<int>(50, 50));
	ParameterSlider::resized();
}


