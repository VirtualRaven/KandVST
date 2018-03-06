#include "MixerComponent.h"

MixerComponent::~MixerComponent()
{
}


MixerComponent::MixerComponent()
{
	__osc1 = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(0, "OSC_MIX_AMP"));
	__osc2 = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(1, "OSC_MIX_AMP"));
	__osc3 = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(2, "OSC_MIX_AMP"));
	__osc4 = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(3, "OSC_MIX_AMP"));
	__masterMixer = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(3, "OSC_MIX_AMP"));

	addAndMakeVisible(__osc1);
	addAndMakeVisible(__osc2);
	addAndMakeVisible(__osc3);
	addAndMakeVisible(__osc4);
	addAndMakeVisible(__masterMixer);

	__osc1->setSliderStyle(Slider::LinearVertical);
	__osc1->setTextBoxStyle(__osc1->TextBoxBelow, true, 55, 15);
	__osc1Label.setText("OSC 1", NotificationType::dontSendNotification);
	__osc1Label.attachToComponent(__osc1, false);
	__osc1Label.setJustificationType(Justification::centred);

	__osc2->setSliderStyle(Slider::LinearVertical);
	__osc2->setTextBoxStyle(__osc2->TextBoxBelow, true, 55, 15);
	__osc2Label.setText("OSC 2", NotificationType::dontSendNotification);
	__osc2Label.attachToComponent(__osc2, false);
	__osc2Label.setJustificationType(Justification::centred);


	__osc3->setSliderStyle(Slider::LinearVertical);
	__osc3->setTextBoxStyle(__osc3->TextBoxBelow, true, 55, 15);
	__osc3Label.setText("OSC 3", NotificationType::dontSendNotification);
	__osc3Label.attachToComponent(__osc3, false);
	__osc3Label.setJustificationType(Justification::centred);

	__osc4->setSliderStyle(Slider::LinearVertical);
	__osc4->setTextBoxStyle(__osc4->TextBoxBelow, true, 55, 15);
	__osc4Label.setText("OSC 4", NotificationType::dontSendNotification);
	__osc4Label.attachToComponent(__osc4, false);
	__osc4Label.setJustificationType(Justification::centred);

	__masterMixer->setSliderStyle(Slider::LinearVertical);
	__masterMixer->setTextBoxStyle(__masterMixer->TextBoxBelow, true, 55, 15);
	__masterLabel.setText("MASTER", NotificationType::dontSendNotification);
	__masterLabel.attachToComponent(__masterMixer, false);
	__masterLabel.setJustificationType(Justification::centred);

}

void MixerComponent::paint(Graphics& g) {

	int width = __bounds.getWidth() * 0.95;
	int height = __bounds.getHeight() * 0.97;
	int fontHeight = __bounds.getHeight() * 0.08;
	int fontSize = __bounds.getHeight() * 0.05;

	g.setColour(Colour::fromRGB(60, 60, 60));
	g.drawRect(Rectangle<int>(10, 10, width, height), 2.0f);
	g.fillRect(Rectangle<int>(10, 10, width, fontHeight));

	Font mixer(fontSize, Font::FontStyleFlags::bold);
	g.setColour(Colours::white);
	g.setFont(mixer);
	g.drawText("MIXER", Rectangle<int>(10, 10, width, fontHeight), Justification::centred, false);

}

void MixerComponent::resized() {
	__bounds = getLocalBounds();

	Rectangle<int> mixerBounds(getLocalBounds().reduced(8)
		.removeFromBottom(__bounds.getHeight() - __bounds.getHeight()*0.2)
		.removeFromTop(__bounds.getHeight() * 0.8)
		.removeFromLeft(__bounds.getWidth() * 0.35));

	__masterMixer->setBounds(mixerBounds);

	Rectangle<int> ampBounds(getLocalBounds().reduced(8));
	ampBounds.removeFromTop(ampBounds.getHeight() * 0.165);
	ampBounds.removeFromLeft(ampBounds.getWidth() * 0.4);

	Rectangle<int> topBounds(ampBounds
		.removeFromTop(ampBounds.getHeight() * 0.45)
		.removeFromLeft(ampBounds.getWidth() * 0.9));

	__osc2->setBounds(topBounds.removeFromRight(topBounds.getWidth() * 0.42));
	__osc1->setBounds(topBounds);
	
	ampBounds.removeFromTop(ampBounds.getHeight() * 0.2);
	
	Rectangle<int> lowerBounds(ampBounds
		.removeFromTop(ampBounds.getHeight())
		.removeFromLeft(ampBounds.getWidth() * 0.9));
	__osc4->setBounds(lowerBounds.removeFromRight(lowerBounds.getWidth() * 0.42));
	__osc3->setBounds(lowerBounds);

}