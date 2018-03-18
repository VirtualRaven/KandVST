#include "../JuceLibraryCode/JuceHeader.h"
#include "MasterPageComponent.h"

MasterComponent::~MasterComponent()
{
}

MasterComponent::MasterComponent() :
	__infoComponent(),
	__lfoComponent1(0),
	__lfoComponent2(1),
	__filterComponent(-1),
	__settingsComponent(),
	__delayComponent()
{
	
	addAndMakeVisible(__infoComponent);
	addAndMakeVisible(__mixerComponent);
	addAndMakeVisible(__filterComponent);
	addAndMakeVisible(__lfoComponent1);
	addAndMakeVisible(__lfoComponent2);

	addAndMakeVisible(__settingsComponent);
	addAndMakeVisible(__delayComponent);

}

void MasterComponent::paint(Graphics& g){
	g.setColour(Colour::fromRGB(40, 40, 40));
	g.fillAll();
}

void MasterComponent::resized() {
	
	__infoComponent.setBounds(getLocalBounds().removeFromTop(300));

	__lfoComponent1.setBounds(Rectangle<int>(0, 8, 250, 135));
	__lfoComponent2.setBounds(Rectangle<int>(0, 160, 250, 135));

	__filterComponent.setBounds(Rectangle<int>(720, 0, 250, 160));
	__settingsComponent.setBounds(Rectangle<int>(720, 160, 100, 135));
	__delayComponent.setBounds(Rectangle<int>(820, 160, 150, 135));

	__mixerComponent.setBounds(Rectangle<int>(0, 290, 970, 260));

}