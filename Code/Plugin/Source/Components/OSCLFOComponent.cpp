#include "../JuceLibraryCode/JuceHeader.h"
#include "OSCLFOComponent.h"

OSCLFOComponent::~OSCLFOComponent()
{
}

OSCLFOComponent::OSCLFOComponent(int ID):
IVSTParameters(ID)
{
	addAndMakeVisible(__freqSelcet = new SelectKnob(*Global->paramHandler->Get<AudioParameterChoice>(ID, "OSC_LFO_FREQ")));
	__freqLabel.setText("FREQUENCY", NotificationType::dontSendNotification);
	__freqLabel.attachToComponent(__freqSelcet, false);
	__freqLabel.setJustificationType(Justification::centred);

	addAndMakeVisible(__ampSelect = new SelectKnob(*Global->paramHandler->Get<AudioParameterChoice>(ID, "OSC_LFO_AMP")));
	__ampLabel.setText("AMPLITUDE", NotificationType::dontSendNotification);
	__ampLabel.attachToComponent(__ampSelect, false);
	__ampLabel.setJustificationType(Justification::centred);
	
}

void OSCLFOComponent::paint(Graphics& g){
	g.setColour(Colour::fromRGB(60, 60, 60));

	int width = __bounds.getWidth();
	int height = __bounds.getHeight();
	int fontHeight = jmin<int>(__bounds.getHeight() * 0.2,25);
	g.drawRect(Rectangle<int>(0, 0, width, height), 2.0f);
	g.fillRect(Rectangle<int>(0, 0, width, fontHeight));
}

void OSCLFOComponent::resized(){

	__bounds = getLocalBounds();
	int fontHeight = jmin<int>(__bounds.getHeight() * 0.2, 25);
	Rectangle<int> sliders(__bounds);
	sliders.removeFromTop(fontHeight);
	sliders.removeFromTop(__freqLabel.getFont().getHeight()+8);
	int size = __bounds.getWidth();
	__freqSelcet->setBounds(sliders.removeFromTop(90));
	sliders.removeFromTop(__freqLabel.getFont().getHeight());
	__ampSelect->setBounds(sliders.removeFromTop(90));

}