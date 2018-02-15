

#include "PluginProcessor.h"
#include "PluginGui.h"
//==============================================================================
PluginGUI::PluginGUI(PluginProcessor& owner)
    : AudioProcessorEditor (owner),
	__tabComponent(TabbedButtonBar::Orientation::TabsAtTop),
	__keyboard(owner.keyboardState,MidiKeyboardComponent::Orientation::horizontalKeyboard),
	__cc()
{
	__tabComponent.addTab("M", Colours::darkgreen, new MasterComponent(), true);

	for (int i = 0; i < 4; i++)
	{
		__tabComponent.addTab(std::to_string(i+1), Colours::darkgrey, new OscillatorPageComponent(i), true);
	}

	__tabComponent.addTab("Console", Colours::darkgrey, &__cc,true);

	addAndMakeVisible(__tabComponent);
	addAndMakeVisible(__keyboard);
	__keyboard.setKeyWidth(__keyboard.getKeyWidth()+10.0f);


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

	__keyboard.setBounds(localBounds.removeFromBottom(100).removeFromRight(getLocalBounds().getWidth() - 150).removeFromLeft(getLocalBounds().getWidth() - 150));
	__keyboard.setCentrePosition(localBounds.getCentreX(), __keyboard.getBounds().getBottom()-(__keyboard.getHeight()/2));	
	localBounds.removeFromBottom(8); //Padding
	__tabComponent.setBounds(localBounds);

}