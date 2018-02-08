#include "EnvelopeComponent.h"



EnvelopeComponent::EnvelopeComponent(int ID):
	IVSTParameters(ID),
	attackLabel(String(), "Attack:"),
	decayLabel(String(), "Decay:"),
	sustainLabel(String(), "Sustain:"),
	releaseLabel(String(), "Release:")
{
	addAndMakeVisible(attack = new ParameterSlider(*Global.paramHandler->GetFloat(__ID, "ENV_ATTACK")));
	attack->setSliderStyle(Slider::LinearVertical);
	attack->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 50, 15);
	addAndMakeVisible(decay = new ParameterSlider(*Global.paramHandler->GetFloat(__ID, "ENV_DECAY")));
	decay->setSliderStyle(Slider::LinearVertical);
	decay->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 50, 15);
	addAndMakeVisible(sustain = new ParameterSlider(*Global.paramHandler->GetFloat(__ID, "ENV_SUSTAIN")));
	sustain->setSliderStyle(Slider::LinearVertical);
	sustain->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 50, 15);
	addAndMakeVisible(release = new ParameterSlider(*Global.paramHandler->GetFloat(__ID, "ENV_RELEASE")));
	release->setSliderStyle(Slider::LinearVertical);
	release->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 50, 15);

	// add some labels for the sliders..
	
	attackLabel.attachToComponent(attack, false);
	attackLabel.setFont(Font(11.0f));
	decayLabel.attachToComponent(decay, false);
	decayLabel.setFont(Font(11.0f));
	sustainLabel.attachToComponent(sustain, false);
	sustainLabel.setFont(Font(11.0f));
	releaseLabel.attachToComponent(release, false);
	releaseLabel.setFont(Font(11.0f));
	setSize(400, 100);
}


EnvelopeComponent::~EnvelopeComponent()
{
}

void EnvelopeComponent::resized()
{
	attack->setBounds(getLocalBounds().removeFromLeft(50));
	decay->setBounds(getLocalBounds().removeFromLeft(100).removeFromRight(50));
	sustain->setBounds(getLocalBounds().removeFromLeft(150).removeFromRight(50));
	release->setBounds(getLocalBounds().removeFromLeft(200).removeFromRight(50));
}
