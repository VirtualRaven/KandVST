#include "../JuceLibraryCode/JuceHeader.h"
#include "LFOComponent.h"

LFOComponent::~LFOComponent()
{
}

LFOComponent::LFOComponent(int ID):
IVSTParameters(ID)
{
	addAndMakeVisible(__ratio = new ParameterSlider(*Global->paramHandler->Get<AudioParameterInt>(__ID, "LFO_RATIO")));
	__ratio->setSliderStyle(Slider::LinearVertical);
	__ratio->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 50, 15);

	addAndMakeVisible(__type = new ParameterSlider(*Global->paramHandler->Get<AudioParameterInt>(__ID, "LFO_TYPE")));
	__type->setSliderStyle(Slider::LinearVertical);
	__type->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 50, 15);

	addAndMakeVisible(__amp = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "LFO_AMOUNT")));
	__amp->setSliderStyle(Slider::LinearVertical);
	__amp->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 50, 15);

	setSize(300, 100);
}

void LFOComponent::paint(Graphics& g){
	
}

void LFOComponent::resized(){
	Rectangle<int> r = getLocalBounds();
	__ratio->setBounds(r.removeFromLeft(100));
	__type->setBounds(r.removeFromLeft(100));
	__amp->setBounds(r.removeFromLeft(100));

}