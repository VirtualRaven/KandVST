
#include "SelectKnob.h"

SelectKnob::SelectKnob(AudioParameterChoice& p) :
	ParameterSlider(p),
	__labels(),
	__param(p)
{
	updateSliderPos();
	setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	setTextBoxStyle(Slider::NoTextBox, false, 0, 0);

	for (int i = 0; i < __param.choices.size(); i++)
	{
		Label* l = new Label(__param.choices[i], __param.choices[i]);
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

void SelectKnob::valueChanged()
{ 
	param.setValueNotifyingHost((float)Slider::getValue()); 
}

void SelectKnob::startedDragging()
{ 
	param.beginChangeGesture(); 
}
void SelectKnob::stoppedDragging()
{ 
	param.endChangeGesture(); 
}

void SelectKnob::paint(Graphics& g) {
	ParameterSlider::paint(g);

	g.setColour(Colour::fromRGB(255, 40, 40));
	g.drawRect(getBounds().reduced(10));
}

void SelectKnob::resized()
{
	Rectangle<int> box(getLocalBounds());
	for (int i = 0; i < __labels.size(); i++)
	{
		__labels.at(i)->setBounds(box);
	}

	ParameterSlider::resized();
}


