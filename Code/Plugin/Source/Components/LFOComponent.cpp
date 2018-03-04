#include "../JuceLibraryCode/JuceHeader.h"
#include "LFOComponent.h"

LFOComponent::~LFOComponent()
{
}

LFOComponent::LFOComponent(int ID):
IVSTParameters(ID)
{
	__ID = ID;
	int boxWidth = 50;
	addAndMakeVisible(__ratio = new ParameterSlider(*Global->paramHandler->Get<AudioParameterInt>(__ID, "LFO_RATIO")));
	__ratio->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__ratio->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, boxWidth, 15);
	__ratioLabel.setText("RATIO", NotificationType::dontSendNotification);
	__ratioLabel.attachToComponent(__ratio, false);
	__ratioLabel.setJustificationType(Justification::centred);

	addAndMakeVisible(__type = new ParameterSlider(*Global->paramHandler->Get<AudioParameterInt>(__ID, "LFO_TYPE")));
	__type->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__type->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, boxWidth, 15);
	__typeLabel.setText("TYPE", NotificationType::dontSendNotification);
	__typeLabel.attachToComponent(__type, false);
	__typeLabel.setJustificationType(Justification::centred);

	addAndMakeVisible(__amp = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "LFO_AMOUNT")));
	__amp->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__amp->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, boxWidth, 15);
	__ampLabel.setText("GAIN", NotificationType::dontSendNotification);
	__ampLabel.attachToComponent(__amp, false);
	__ampLabel.setJustificationType(Justification::centred);

	addAndMakeVisible(__toggleLfo = new ParameterButton(*Global->paramHandler->Get<AudioParameterBool>(__ID, "LFO_EN")));
	__toggleLfo->setButtonText("LFO " + std::to_string(__ID+1));
	setSize(260, 140);
}

void LFOComponent::paint(Graphics& g){
	g.setColour(Colour::fromRGB(60, 60, 60));
	g.drawRect(Rectangle<int>(8,10,255,130), 2.0f);
	g.fillRect(Rectangle<int>(8, 10, 255, 35));
}

void LFOComponent::resized(){
	int size = 50;
	int gap = 25;
	Rectangle<int> r = getLocalBounds().reduced(8);
	r.removeFromTop(7);
	r.removeFromLeft(30);
	__toggleLfo->setBounds(r.removeFromTop(25).removeFromLeft(195));

	Rectangle<int> sliders(getLocalBounds().reduced(8));
	sliders.removeFromTop(60);
	sliders.removeFromLeft(gap);
	__ratio->setBounds(sliders.removeFromLeft(size).removeFromTop(size+15));
	sliders.removeFromLeft(gap);
	__type->setBounds(sliders.removeFromLeft(size).removeFromTop(size+15));
	sliders.removeFromLeft(gap);
	__amp->setBounds(sliders.removeFromLeft(size).removeFromTop(size+15));

}