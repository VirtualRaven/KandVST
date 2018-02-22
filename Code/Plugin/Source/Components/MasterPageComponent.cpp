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

	Image titleImage = ImageFileFormat::loadFrom(Resources::Images::Title_png, sizeof(Resources::Images::Title_png));
	g.setOpacity(1.0f);
	g.drawImageAt(titleImage, 404, 20, false);
}

void MasterComponent::resized(){
	//__presetComponent.setCentrePosition(getLocalBounds().getCentre());
	__infoComponent.setBounds(getLocalBounds());
	__infoComponent.setCentrePosition(getLocalBounds().getCentre());
}