#include "OscillatorPageComponent.h"

OscillatorPageComponent::~OscillatorPageComponent()
{
}


OscillatorPageComponent::OscillatorPageComponent(int ID):
IVSTParameters(ID),
__envComponent(ID)

{
	addAndMakeVisible(__envComponent);
	addAndMakeVisible(__oscillator);
}

void OscillatorPageComponent::paint(Graphics& g){
	g.setColour(Colour::fromRGB(56, 56, 56));
	g.fillAll();
}

void OscillatorPageComponent::resized(){
	Rectangle<int> env(320,50,300, 200);
	__envComponent.setBounds(env);

	__oscillator.setBounds(getLocalBounds());

}