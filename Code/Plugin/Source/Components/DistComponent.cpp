#include "../JuceLibraryCode/JuceHeader.h"
#include "DistComponent.h"

DistComponent::~DistComponent()
{
}

DistComponent::DistComponent(int ID, GLOBAL * global) :
	IVSTParameters(ID),
	__ID(ID)
{
	Global=global;
	addAndMakeVisible(__threshold = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "DIST_TRSH"), Global));
	__threshold->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__threshold->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 50, 15);


	addAndMakeVisible(__toggleDist = new ParameterButton(*Global->paramHandler->Get<AudioParameterBool>(__ID, "DIST_EN")));
	__toggleDist->setButtonText("Dist " + std::to_string(__ID + 1));
	setSize(140, 140);
}

void DistComponent::paint(Graphics& g) {
	g.setColour(Colour::fromRGB(60, 60, 60));

	int width = __bounds.getWidth();
	int height = __bounds.getHeight();
	int fontHeight = __bounds.getHeight() * 0.2;
	g.drawRect(Rectangle<int>(0, 0, width, height), 2.0f);
	g.fillRect(Rectangle<int>(0, 0, width, fontHeight));
}

void DistComponent::resized() {
	__bounds = getLocalBounds();

	int fontHeight = __bounds.getHeight() * 0.2;
	Rectangle<int> distButtonBounds(0, 0, __bounds.getWidth(), fontHeight);
	__toggleDist->setBounds(distButtonBounds);

	Rectangle<int> sliders(__bounds.reduced(8));
	int size = sliders.getWidth();
	sliders.removeFromTop(fontHeight);
	__threshold->setBounds(sliders.removeFromLeft(size));
}