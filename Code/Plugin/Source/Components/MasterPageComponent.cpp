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
	__settingsComponent()
{
	
	addAndMakeVisible(__infoComponent);
	addAndMakeVisible(__presetComponent);
	addAndMakeVisible(__mixerComponent);
	addAndMakeVisible(__filterComponent);
	addAndMakeVisible(__lfoComponent1);
	addAndMakeVisible(__lfoComponent2);

	addAndMakeVisible(__settingsComponent);

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

	__filterComponent.setBounds(Rectangle<int>(740, 370, 230, 160));
	__lfoComponent1.setBounds(Rectangle<int>(0,8,250,145));
	__lfoComponent2.setBounds(Rectangle<int>(0, 155, 250, 145));

	__settingsComponent.setBounds(Rectangle<int>(8, 302, 265, 145));
}