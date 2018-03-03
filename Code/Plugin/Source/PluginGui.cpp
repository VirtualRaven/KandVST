

#include "PluginProcessor.h"
#include "PluginGui.h"
//==============================================================================
PluginGUI::PluginGUI(PluginProcessor& owner)
	: AudioProcessorEditor(owner),
	ourLookAndFeel(),
	__tabComponent(TabbedButtonBar::Orientation::TabsAtTop),
	__keyboard(owner.keyboardState,MidiKeyboardComponent::Orientation::horizontalKeyboard),
	__cc()
{
	setResizable(false, false);
	setSize(1000, 720);
	__loadingImage.setImage(ImageFileFormat::loadFrom(Resources::Images::loading_png,sizeof(Resources::Images::loading_png)));

	addAndMakeVisible(__loadingImage);
	__loadingImage.setBounds(getLocalBounds());

}


void PluginGUI::InitializeGui()
{
	removeAllChildren();
	setLookAndFeel(&ourLookAndFeel);

	__tabComponent.addTab("M", Colours::darkgreen, new MasterComponent(), true);

	for (int i = 0; i < 4; i++)
	{
		__tabComponent.addTab(std::to_string(i + 1), Colours::darkgrey, new OscillatorPageComponent(i), true);
	}

	__tabComponent.addTab("Console", Colours::darkgrey, &__cc, true);

	addAndMakeVisible(__tabComponent);
	addAndMakeVisible(__keyboard);
	__keyboard.setKeyWidth(__keyboard.getKeyWidth() + 10.0f);


}



PluginGUI::~PluginGUI()
{
	//Has to unset our lookAndFeel before it is destroyed
	this->setLookAndFeel(nullptr);
}

//==============================================================================
void PluginGUI::paint (Graphics& g)
{
    g.setColour (Colour::fromRGB(36,36,36));
    g.fillAll();

	Image titleImage = ImageFileFormat::loadFrom(Resources::Images::Title3_png, sizeof(Resources::Images::Title3_png));
	g.setOpacity(1.0f);
	g.drawImageAt(titleImage, 1000, 0, false);
}

void PluginGUI::resized()
{
	Rectangle<int> localBounds (getLocalBounds().reduced(8));

	__keyboard.setBounds(localBounds.removeFromBottom(100).removeFromRight(getLocalBounds().getWidth() - 150).removeFromLeft(getLocalBounds().getWidth() - 150));
	__keyboard.setCentrePosition(localBounds.getCentreX(), __keyboard.getBounds().getBottom()-(__keyboard.getHeight()/2));	
	localBounds.removeFromBottom(8); //Padding
	__tabComponent.setBounds(localBounds);

}



