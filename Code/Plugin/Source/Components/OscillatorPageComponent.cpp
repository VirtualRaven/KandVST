
#include "OscillatorPageComponent.h"

OscillatorComponent::~OscillatorComponent()
{
}


OscillatorComponent::OscillatorComponent(int ID):
IVSTParameters(ID),
__envComponent(ID)
{
	addAndMakeVisible(__envComponent);
}

void OscillatorComponent::paint(Graphics& g){
	g.setColour(Colour::fromRGB(56, 56, 56));
	g.fillAll();
}

void OscillatorComponent::resized(){
	__envComponent.setBounds(getLocalBounds());

}