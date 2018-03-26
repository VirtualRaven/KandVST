

#include "PluginProcessor.h"
#include "PluginGUI.h"
//==============================================================================
PluginGUI::PluginGUI(PluginProcessor& owner, GLOBAL* global)
	: AudioProcessorEditor(owner),
	ourLookAndFeel(),
	__tabComponent(TabbedButtonBar::Orientation::TabsAtTop),
	__keyboard(owner.keyboardState,MidiKeyboardComponent::Orientation::horizontalKeyboard),
	__cc(global),
	__owner(&owner),
	__guiInit(false)
{
	Global=global;
	setResizable(false, false);
	setSize(1000, 720);
	__loadingImage.setImage(ImageFileFormat::loadFrom(Resources::Images::loading_png,sizeof(Resources::Images::loading_png)));

	addAndMakeVisible(__loadingImage);
	__loadingImage.setBounds(getLocalBounds());
	addKeyListener(this);
	startTimerHz(60);
}


void PluginGUI::InitializeGui()
{
	if (__guiInit)
		return;
	__guiInit = true;
	const MessageManagerLock mmLock;
	removeAllChildren();
	setLookAndFeel(&ourLookAndFeel);
	__tabComponent.setColour(TabbedComponent::ColourIds::outlineColourId, Colour::fromRGBA(0, 0, 0, 0));
	__tabComponent.setTabBarDepth(35);
	__tabComponent.setOutline(0);
	__tabComponent.getTabbedButtonBar().setColour(TabbedButtonBar::ColourIds::tabOutlineColourId, Colours::transparentWhite);
	__tabComponent.addTab("M", Colour::fromRGB(200,200,200), new MasterComponent(Global), true);

	for (int i = 0; i < 4; i++)
	{
		__tabComponent.addTab(std::to_string(i + 1), Colour::fromRGB(1,1,1), new OscillatorPageComponent(i, Global), true);
	}

	__tabComponent.addTab("C", Colours::darkgrey, &__cc, true);
	addAndMakeVisible(__tabComponent);
	addAndMakeVisible(__keyboard);
	__keyboard.setKeyWidth(__keyboard.getKeyWidth() + 10.0f);
}

bool PluginGUI::keyPressed(const KeyPress & /*key*/, Component * /*originatingComponent*/)
{
	return false;
}

bool PluginGUI::keyStateChanged(bool isKeyDown, Component * /*originatingComponent*/)
{
	return __keyboard.keyStateChanged(isKeyDown);
}

void PluginGUI::timerCallback()
{
	if (!__guiInit&&__owner->isReady()) {
		InitializeGui();
		stopTimer();
	}
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
	g.drawImageAt(titleImage, 780, 0, false);

}

void PluginGUI::resized()
{
	Rectangle<int> localBounds (getLocalBounds().reduced(8));

	__keyboard.setBounds(localBounds.removeFromBottom(100).removeFromRight(getLocalBounds().getWidth() - 150).removeFromLeft(getLocalBounds().getWidth() - 150));
	__keyboard.setCentrePosition(localBounds.getCentreX(), __keyboard.getBounds().getBottom()-(__keyboard.getHeight()/2));	
	localBounds.removeFromBottom(8); //Padding
	__tabComponent.setBounds(localBounds);

}



