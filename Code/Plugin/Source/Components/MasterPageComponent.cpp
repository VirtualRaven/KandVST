#include "../JuceLibraryCode/JuceHeader.h"
#include "MasterPageComponent.h"

MasterComponent::~MasterComponent()
{
}

MasterComponent::MasterComponent():
	__infoComponent()
{
	addAndMakeVisible(__infoComponent);
	addAndMakeVisible(__presetComponent);
	addAndMakeVisible(__mixerComponent);

}

void MasterComponent::paint(Graphics& g){
	g.setColour(Colour::fromRGB(40, 40, 40));
	g.fillAll();

	Font presets(20, Font::FontStyleFlags::bold);
	g.setFont(presets);
	g.setColour(Colours::white);
	g.drawSingleLineText("PRESETS", 500, 300, Justification::centred);
	
}

void MasterComponent::resized() {
	__presetComponent.setBounds(getLocalBounds().removeFromBottom(260).removeFromRight(680).removeFromLeft(400));
	__infoComponent.setBounds(getLocalBounds().removeFromTop(300));
	__mixerComponent.setBounds(getLocalBounds().reduced(8));
	
}