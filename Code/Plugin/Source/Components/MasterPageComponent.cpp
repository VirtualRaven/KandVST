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
	addAndMakeVisible(__presetComponent);
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
	__lfoComponent1.setBounds(Rectangle<int>(0,8,250,135));
	__lfoComponent2.setBounds(Rectangle<int>(0, 145, 250, 135));

	__settingsComponent.setBounds(Rectangle<int>(0, 282, 100, 135));

	__delayComponent.setBounds(Rectangle<int>(100, 282, 150, 135));
}