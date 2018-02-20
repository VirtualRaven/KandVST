#include "OscillatorPageComponent.h"

OscillatorPageComponent::~OscillatorPageComponent()
{
}


OscillatorPageComponent::OscillatorPageComponent(int ID):
IVSTParameters(ID),
__envComponent(ID),
__oscillator(ID),
__lfo(ID)
{
	__amplitude = new  ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(ID,"OSC_MIX_AMP"));
	addAndMakeVisible(__envComponent);
	addAndMakeVisible(__oscillator);
	addAndMakeVisible(__lfo);
	addAndMakeVisible(__amplitude);

	__amplitude->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__amplitude->setTextBoxStyle(__amplitude->NoTextBox, true, 10, 10);
	__ampLabel.setText("Amplitude", NotificationType::dontSendNotification);
	__ampLabel.attachToComponent(__amplitude, false);

	addAndMakeVisible(__toggleOsc = new ParameterButton(*Global->paramHandler->Get<AudioParameterBool>(__ID, "OSC_MIX_EN")));
	__toggleOsc->setButtonText("Oscillator");
	
}

void OscillatorPageComponent::paint(Graphics& g){
	g.setColour(Colour::fromRGB(56, 56, 56));
	g.fillAll();
}

void OscillatorPageComponent::resized(){
	__toggleOsc->setBounds(Rectangle<int>(16, 16, 75, 25));

	Rectangle<int> env(340,50,300, 200);
	__envComponent.setBounds(env);
	__lfo.setBounds(getLocalBounds().removeFromRight(182).removeFromTop(250));
	__oscillator.setBounds(getLocalBounds().removeFromLeft(320));


	Rectangle<int> amplitudeBounds(getLocalBounds().reduced(16));
	__amplitude->setBounds(amplitudeBounds.removeFromRight(100).removeFromBottom(100));
}