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
	__lpFreq = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(ID, "FILTER_LP"));
	__hpFreq = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(ID, "FILTER_HP"));

	addAndMakeVisible(__envComponent);
	addAndMakeVisible(__oscillator);
	addAndMakeVisible(__lfo);
	addAndMakeVisible(__amplitude);
	addAndMakeVisible(__lpFreq);
	addAndMakeVisible(__hpFreq);

	//__amplitude->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	//__amplitude->setTextBoxStyle(__amplitude->NoTextBox, true, 10, 10);
	//__ampLabel.setText("Amplitude", NotificationType::dontSendNotification);
	//__ampLabel.attachToComponent(__amplitude, false);
	//__ampLabel.setJustificationType(juce::Justification::centred);

	__lpFreq->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__lpFreq->setTextBoxStyle(__lpFreq->NoTextBox, true, 10, 10);
	__lpFreq->setSkewFactorFromMidPoint(0.15);
	__lpLabel.setText("LP", NotificationType::dontSendNotification);
	__lpLabel.attachToComponent(__lpFreq, false);
	__lpLabel.setJustificationType(juce::Justification::centred);

	__hpFreq->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__hpFreq->setTextBoxStyle(__hpFreq->NoTextBox, true, 10, 10);
	__hpFreq->setSkewFactorFromMidPoint(0.15);
	__hpLabel.setText("HP", NotificationType::dontSendNotification);
	__hpLabel.attachToComponent(__hpFreq, false);
	__hpLabel.setJustificationType(juce::Justification::centred);
	
	addAndMakeVisible(__toggleOsc = new ParameterButton(*Global->paramHandler->Get<AudioParameterBool>(__ID, "OSC_MIX_EN")));
	__toggleOsc->setButtonText("Oscillator");
	
}

void OscillatorPageComponent::paint(Graphics& g){
	g.setColour(Colour::fromRGB(40, 40, 40));
	g.fillAll();

	g.setColour(Colours::white);
	g.setFont(20.0f);
	g.drawText("ENVELOPE", Rectangle<float>(350,8,300,25), Justification::centred, false);
}

void OscillatorPageComponent::resized(){


	Rectangle<int> bounds(getLocalBounds().reduced(8));
	__toggleOsc->setBounds(bounds.removeFromTop(25).removeFromLeft(75));
	__oscillator.setBounds(bounds.removeFromLeft(400));
	__envComponent.setBounds(bounds.removeFromLeft(250));
	__lfo.setBounds(getLocalBounds().reduced(8).removeFromRight(200).removeFromTop(250));

	Rectangle<int> amplitudeBounds(getLocalBounds().reduced(16));
	//__amplitude->setBounds(amplitudeBounds.removeFromRight(100).removeFromBottom(100));
	__lpFreq->setBounds(amplitudeBounds.removeFromRight(50).removeFromBottom(50));
	__hpFreq->setBounds(amplitudeBounds.removeFromRight(50).removeFromBottom(50));
}