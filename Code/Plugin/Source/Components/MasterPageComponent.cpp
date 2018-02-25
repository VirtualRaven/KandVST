#include "../JuceLibraryCode/JuceHeader.h"
#include "MasterPageComponent.h"

MasterComponent::~MasterComponent()
{
}

MasterComponent::MasterComponent():
	__infoComponent()
{
	addAndMakeVisible(__infoComponent);
	//addAndMakeVisible(__presetComponent);
}

void MasterComponent::paint(Graphics& g){
	g.setColour(Colour::fromRGB(40, 40, 40));
	g.fillAll();
}

void MasterComponent::resized() {
	//__presetComponent.setCentrePosition(getLocalBounds().getCentre());
	__infoComponent.setBounds(getLocalBounds());
}