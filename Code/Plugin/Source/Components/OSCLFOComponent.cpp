#include "../JuceLibraryCode/JuceHeader.h"
#include "OSCLFOComponent.h"

OSCLFOComponent::~OSCLFOComponent()
{
}

OSCLFOComponent::OSCLFOComponent(int ID,GLOBAL*global):
IVSTParameters(ID)
{
	Global = global;
	addAndMakeVisible(__freqSelect = new ParameterSlider(*Global->paramHandler->Get<AudioParameterChoice>(ID, "OSC_LFO_FREQ"),global));
	__freqLabel.setText("FREQUENCY", NotificationType::dontSendNotification);
	__freqLabel.attachToComponent(__freqSelect, false);
	__freqLabel.setJustificationType(Justification::centred);

	addAndMakeVisible(__ampSelect = new ParameterSlider(*Global->paramHandler->Get<AudioParameterChoice>(ID, "OSC_LFO_AMP"),global));
	__ampLabel.setText("AMPLITUDE", NotificationType::dontSendNotification);
	__ampLabel.attachToComponent(__ampSelect, false);
	__ampLabel.setJustificationType(Justification::centred);
	
	addAndMakeVisible(__lpSelect = new ParameterSlider(*Global->paramHandler->Get<AudioParameterChoice>(ID, "FILTER_LP_LFO"), global));
	__lpLabel.setText("LOW-PASS", NotificationType::dontSendNotification);
	__lpLabel.attachToComponent(__lpSelect, false);
	__lpLabel.setJustificationType(Justification::centred);

	addAndMakeVisible(__hpSelect = new ParameterSlider(*Global->paramHandler->Get<AudioParameterChoice>(ID, "FILTER_HP_LFO"), global));
	__hpLabel.setText("HIGH-PASS", NotificationType::dontSendNotification);
	__hpLabel.attachToComponent(__hpSelect, false);
	__hpLabel.setJustificationType(Justification::centred);
	
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
	sliders.removeFromTop(__freqLabel.getFont().getHeight()+10);
	int size = __bounds.getWidth() / 2.0;
	Rectangle<int> lfo(sliders.removeFromTop(100));
	__freqSelect->setBounds(lfo.removeFromRight(size));
	__ampSelect->setBounds(lfo.removeFromRight(size));
	sliders.removeFromTop(15);
	Rectangle<int> filters(sliders);
	__lpSelect->setBounds(filters.removeFromRight(size));
	__hpSelect->setBounds(filters.removeFromRight(size));

}