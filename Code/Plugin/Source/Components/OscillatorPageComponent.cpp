#include "OscillatorPageComponent.h"

OscillatorPageComponent::~OscillatorPageComponent()
{
}


OscillatorPageComponent::OscillatorPageComponent(int ID):
IVSTParameters(ID),
__envComponent(ID),
__oscillator(ID),
__filterComponent(ID),
__osclfoComponent(ID)
{
	__amplitude = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(ID,"OSC_MIX_AMP"));

	addAndMakeVisible(__envComponent);
	addAndMakeVisible(__oscillator);
	addAndMakeVisible(__amplitude);
	addAndMakeVisible(__filterComponent);
	addAndMakeVisible(__osclfoComponent);

	addAndMakeVisible(__toggleOsc = new ParameterButton(*Global->paramHandler->Get<AudioParameterBool>(__ID, "OSC_MIX_EN")));
	__toggleOsc->setButtonText("Oscillator");
	
}

void OscillatorPageComponent::paint(Graphics& g){
	g.setColour(Swatch::background);
	g.fillAll();

}

void OscillatorPageComponent::resized(){


	Rectangle<int> bounds(getLocalBounds().reduced(8));
	__toggleOsc->setBounds(bounds.removeFromTop(25).removeFromLeft(100));
	bounds.removeFromTop(5);
	__oscillator.setBounds(bounds.removeFromLeft(400));
	bounds.removeFromLeft(16);
	__envComponent.setBounds(bounds.removeFromLeft(400-32));
	//__lfo.setBounds(getLocalBounds().reduced(8).removeFromRight(200).removeFromTop(250));
	bounds.removeFromLeft(15);
	Rectangle<int> filterBounds(bounds.removeFromBottom(105));
	//__filterComponent.setBounds(Rectangle<int>(820,450,150,105));
	__filterComponent.setBounds(filterBounds);
	bounds.removeFromBottom(8);
	__osclfoComponent.setBounds(bounds);
}