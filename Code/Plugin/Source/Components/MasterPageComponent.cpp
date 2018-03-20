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
	g.setColour(Swatch::background);
	g.fillAll();
}

void MasterComponent::resized() {
	Rectangle<int> bounds = getLocalBounds().reduced(8);

	__mixerComponent.setBounds(bounds.removeFromBottom(260));
	bounds.removeFromBottom(8);
	//__infoComponent.setBounds(getLocalBounds().removeFromTop(300));
	Rectangle<int> lfoBounds = bounds.removeFromLeft(250);
	int lfoH = (lfoBounds.getHeight() - 8) / 2;
	__lfoComponent1.setBounds(lfoBounds.removeFromTop(lfoH));
	lfoBounds.removeFromTop(8);
	__lfoComponent2.setBounds(lfoBounds.removeFromTop(lfoH));

	Rectangle<int> rightColumn = bounds.removeFromRight(250);

	__filterComponent.setBounds(rightColumn.removeFromTop(rightColumn.getHeight()*0.5f));
	rightColumn.removeFromTop(8);
	
	__settingsComponent.setBounds(rightColumn.removeFromLeft(rightColumn.getWidth()*0.40f));
	rightColumn.removeFromLeft(8);
	__delayComponent.setBounds(rightColumn);
	bounds.reduce(8, 0);
	__infoComponent.setBounds(bounds);

	

	/*
	

	__lfoComponent1.setBounds(Rectangle<int>(0, 8, 250, 135));
	__lfoComponent2.setBounds(Rectangle<int>(0, 160, 250, 135));

	__filterComponent.setBounds(Rectangle<int>(720, 0, 250, 160));
	__settingsComponent.setBounds(Rectangle<int>(720, 160, 100, 135));
	__delayComponent.setBounds(Rectangle<int>(820, 160, 150, 135));

	__mixerComponent.setBounds(Rectangle<int>(0, 290, 970, 260));*/

}