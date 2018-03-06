#include "../JuceLibraryCode/JuceHeader.h"
#include "MasterPageComponent.h"

MasterComponent::~MasterComponent()
{
}

MasterComponent::MasterComponent() :
	__infoComponent(),
	__lfoComponent1(0),
	__lfoComponent2(1)
{
	
	addAndMakeVisible(__infoComponent);
	addAndMakeVisible(__presetComponent);
	addAndMakeVisible(__mixerComponent);
	addAndMakeVisible(__filterComponent);
	addAndMakeVisible(__lfoComponent1);
	addAndMakeVisible(__lfoComponent2);

}

void MasterComponent::paint(Graphics& g){
	g.setColour(Colour::fromRGB(40, 40, 40));
	g.fillAll();

	Font presets(20, Font::FontStyleFlags::bold);
	g.setFont(presets);
	g.setColour(Colours::white);
	g.drawSingleLineText("PRESETS", 500, 300, Justification::horizontallyCentred);
}

void MasterComponent::resized() {
	__presetComponent.setBounds(getLocalBounds().removeFromBottom(260).removeFromRight(680).removeFromLeft(400));
	__infoComponent.setBounds(getLocalBounds().removeFromTop(300));
	__mixerComponent.setBounds(Rectangle<int>(740,10,230,360));
	__filterComponent.setBounds(Rectangle<int>(740, 370, 230, 150));
	__lfoComponent1.setBounds(Rectangle<int>(8,8,265,145));
	__lfoComponent2.setBounds(Rectangle<int>(8, 155, 265, 145));
	
}