#include "../JuceLibraryCode/JuceHeader.h"
#include "LFOComponent.h"

LFOComponent::~LFOComponent()
{
}

LFOComponent::LFOComponent(int ID):
IVSTParameters(ID)
{
	__ID = ID;
	addAndMakeVisible(__ratio = new ParameterSlider(*Global->paramHandler->Get<AudioParameterInt>(__ID, "LFO_RATIO")));
	__ratio->setSliderStyle(Slider::LinearVertical);
	__ratio->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 50, 15);

	addAndMakeVisible(__type = new ParameterSlider(*Global->paramHandler->Get<AudioParameterInt>(__ID, "LFO_TYPE")));
	__type->setSliderStyle(Slider::LinearVertical);
	__type->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 50, 15);

	addAndMakeVisible(__amp = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "LFO_AMOUNT")));
	__amp->setSliderStyle(Slider::LinearVertical);
	__amp->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 50, 15);

	addAndMakeVisible(__toggleLfo = new ParameterButton(*Global->paramHandler->Get<AudioParameterBool>(__ID, "LFO_EN")));
	__toggleLfo->setButtonText("LFO");

	setSize(300,200);
}

void LFOComponent::paint(Graphics&){
	
}

void LFOComponent::resized(){
	Rectangle<int> r = getLocalBounds();
	__toggleLfo->setBounds(r.removeFromTop(25).removeFromRight(150));
	Rectangle<int> sliders(r);
	sliders.removeFromLeft(50);
	__ratio->setBounds(sliders.removeFromLeft(50));
	__type->setBounds(sliders.removeFromLeft(50));
	__amp->setBounds(sliders.removeFromLeft(50));

}