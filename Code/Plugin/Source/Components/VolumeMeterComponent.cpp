#include "VolumeMeterComponent.h"


VolumeMeterComponent::~VolumeMeterComponent()
{
	stopTimer();
}
VolumeMeterComponent::VolumeMeterComponent(GLOBAL * global)
{
	Global = global;

	startTimer(30);
	rects.reserve(8);

	rects.push_back(__01 = Rectangle<int>(40, 20));
	rects.push_back(__02 = Rectangle<int>(__01));
	rects.push_back(__11 = Rectangle<int>(__01));
	__12 = Rectangle<int>(__01);
	__21 = Rectangle<int>(__01);
	__22 = Rectangle<int>(__01);
	__31 = Rectangle<int>(__01);
	__32 = Rectangle<int>(__01);

}

void VolumeMeterComponent::paint(Graphics & g) {
	Rectangle<int> rect();
}

void VolumeMeterComponent::resized() {
	Rectangle<int> bounds = getLocalBounds();
}

void VolumeMeterComponent::timerCallback() {

}
