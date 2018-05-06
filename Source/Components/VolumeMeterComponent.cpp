#include "VolumeMeterComponent.h"

VolumeMeterComponent::~VolumeMeterComponent()
{

}
VolumeMeterComponent::VolumeMeterComponent(GLOBAL * global)
{
	Global = global;
	rects.reserve(8);

	rects.push_back(__0 = Rectangle<int>(50, 25));
	rects.push_back(__1 = Rectangle<int>(__0));
	rects.push_back(__2 = Rectangle<int>(__0));
	rects.push_back(__3 = Rectangle<int>(__0));
	rects.push_back(__4 = Rectangle<int>(__0));
	rects.push_back(__5 = Rectangle<int>(__0));
	rects.push_back(__6 = Rectangle<int>(__0));
	startTimerHz(40);
}

void VolumeMeterComponent::paint(Graphics & g) 
{
		int y = 185;
		int x = getLocalBounds().reduced(5).getX();
		int i = 0;
		for(Rectangle<int> rec :  rects) {
			rec.setX(x);
			rec.setY(y);

			g.setColour(Swatch::white);
			g.drawRect(rec, 2);

			
			g.setColour(Swatch::accentOrange.brighter(0.4f).withSaturation(1.2f));
			g.fillRect(rec);

			g.setColour(Swatch::white);
			g.drawText(std::to_string(__db), rec, Justification::centred, false);
			y -= 30;
			i++;
		}
	}

void VolumeMeterComponent::timerCallback()
{
	if (Global->paramHandler->Get<AudioParameterFloat>(-1, "DECIBEL") != nullptr) {
		__db = *Global->paramHandler->Get<AudioParameterFloat>(-1, "DECIBEL");
		repaint();
	}
}

