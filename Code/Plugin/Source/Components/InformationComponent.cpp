#include "../JuceLibraryCode/JuceHeader.h"
#include "InformationComponent.h"

InformationComponent::~InformationComponent()
{
}

InformationComponent::InformationComponent(){

}

void InformationComponent::paint (Graphics& g){
    Rectangle<int> infoWindow(345,95,305,130);
    g.setColour(Colour::fromRGB(20,40,60));
    g.fillAll();
    g.drawRect(infoWindow);
}

void InformationComponent::resized(){
    //set the bounds of any child components here.
}
