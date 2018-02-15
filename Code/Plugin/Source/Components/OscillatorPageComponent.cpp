#include "OscillatorPageComponent.h"

OscillatorPageComponent::~OscillatorPageComponent()
{
}


OscillatorPageComponent::OscillatorPageComponent(int ID):
IVSTParameters(ID),
__envComponent(ID),
__oscillator(ID),
__lfo(ID)
{
	addAndMakeVisible(__envComponent);
	addAndMakeVisible(__oscillator);
	addAndMakeVisible(__lfo);
}

void OscillatorPageComponent::paint(Graphics& g){
	g.setColour(Colour::fromRGB(56, 56, 56));
	g.fillAll();
}

void OscillatorPageComponent::resized(){
	Rectangle<int> env(340,50,300, 200);
	__envComponent.setBounds(env);
	__lfo.setBounds(getLocalBounds().removeFromRight(300).removeFromTop(100));
	__oscillator.setBounds(getLocalBounds().removeFromLeft(320));

}