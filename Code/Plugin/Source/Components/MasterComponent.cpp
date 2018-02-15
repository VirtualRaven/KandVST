#include "../JuceLibraryCode/JuceHeader.h"
#include "MasterComponent.h"

MasterComponent::~MasterComponent()
{
}


MasterComponent::MasterComponent():
	__infoComponent()
{
	addAndMakeVisible(__infoComponent);
}

void MasterComponent::paint(Graphics& g){
	g.setColour(Colour::fromRGB(56, 56, 56));
	g.fillAll();
}

void MasterComponent::resized(){
	__infoComponent.setBounds(getLocalBounds());
	__infoComponent.setCentrePosition(getLocalBounds().getCentre());
}