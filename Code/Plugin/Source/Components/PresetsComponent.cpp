#include "../JuceLibraryCode/JuceHeader.h"
#include "PresetsComponent.h"

PresetsComponent::~PresetsComponent()
{
}

PresetsComponent::PresetsComponent():
__rowModel(&__presets)
{ 
	__refresh.addListener(this);
	Image refImage = ImageFileFormat::loadFrom(Resources::Icons::refresh_png, sizeof(Resources::Icons::refresh_png));
	__refresh.setImages(false, true, true, refImage, 1.0f, Colours::transparentBlack, refImage, 0.7f, Colours::transparentBlack, refImage, 0.5f, Colours::transparentBlack);
	addAndMakeVisible(__presets);
	addAndMakeVisible(__refresh);
	//addAndMakeVisible(__delete);
	//addAndMakeVisible(__save);

	setSize(200, 200);
	__presets.setModel(&__rowModel);
}
void PresetsComponent::resized()
{
	Rectangle<int> localBounds(getLocalBounds().reduced(8));
	__presets.setBounds(localBounds);
	Rectangle<int> refBtn(localBounds);
	__refresh.setBounds(refBtn.removeFromBottom(25).removeFromRight(25));
	
}

void PresetsComponent::buttonClicked(Button * btn)
{
	if (btn == &__refresh)
	{
		Global->presetManager->RefreshPresets();
		__presets.updateContent();
	}
}