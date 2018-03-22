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
	Rectangle<int> lfoBounds = bounds.removeFromLeft(250);
	int lfoH = (lfoBounds.getHeight() - 8) / 2;
	__lfoComponent1.setBounds(lfoBounds.removeFromTop(lfoH));
	lfoBounds.removeFromTop(8);
	__lfoComponent2.setBounds(lfoBounds.removeFromTop(lfoH));

	Rectangle<int> rightColumn = bounds.removeFromRight(250);
	__settingsComponent.setBounds(rightColumn.removeFromLeft(rightColumn.getWidth()*0.40f));
	
	Rectangle<int> topPart = rightColumn.removeFromTop(rightColumn.getHeight()*0.5f);
	topPart.removeFromLeft(8);
	__filterComponent.setBounds(topPart);

	rightColumn.removeFromTop(8);
	
	rightColumn.removeFromLeft(8);
	__delayComponent.setBounds(rightColumn);
	bounds.reduce(8, 0);
	__infoComponent.setBounds(bounds);

}