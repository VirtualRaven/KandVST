

#include "PluginProcessor.h"
#include "PluginGui.h"
//==============================================================================
PluginGUI::PluginGUI(PluginProcessor& owner)
    : AudioProcessorEditor (owner),
	__tabComponent(TabbedButtonBar::Orientation::TabsAtTop),
	__keyboard(owner.keyboardState,MidiKeyboardComponent::Orientation::horizontalKeyboard)
{
	__tabComponent.addTab("M", Colours::darkgreen, new MasterComponent(), true);

	for (int i = 0; i < 1; i++)
	{
		__tabComponent.addTab(std::to_string(i+1), Colours::darkgrey, new OscillatorComponent(i), true);
	}

	addAndMakeVisible(__tabComponent);
	addAndMakeVisible(__keyboard);


	setResizable(false, false);
	setSize(1280, 720);
}

PluginGUI::~PluginGUI()
{
}

//==============================================================================
void PluginGUI::paint (Graphics& g)
{
    g.setColour (Colour::fromRGB(36,36,36));
    g.fillAll();
}

void PluginGUI::resized()
{
	Rectangle<int> localBounds (getLocalBounds().reduced(8));

	__keyboard.setBounds(localBounds.removeFromBottom(70).removeFromRight(getLocalBounds().getWidth() - 100).removeFromLeft(getLocalBounds().getWidth() - 100));
	localBounds.removeFromBottom(8); //Padding
	__tabComponent.setBounds(localBounds);

}