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
	__toggleLfo->addListener(this);
	__toggleLfo->setButtonText("LFO: Disabled");

	setSize(300,200);
}

void LFOComponent::paint(Graphics& g){
	
}

void LFOComponent::resized(){
	Rectangle<int> r = getLocalBounds();
	__toggleLfo->setBounds(r.removeFromTop(50));
	__ratio->setBounds(r.removeFromLeft(50));
	__type->setBounds(r.removeFromLeft(50));
	__amp->setBounds(r.removeFromLeft(50));

}

void LFOComponent::buttonClicked(Button* button) {
	__toggleLfo->buttonClicked(__toggleLfo);
	if (__toggleLfo->getValue()) {
		__toggleLfo->setButtonText("LFO: Enabled");
	} 
	else {
			__toggleLfo->setButtonText("LFO: Disabled");
		}
}