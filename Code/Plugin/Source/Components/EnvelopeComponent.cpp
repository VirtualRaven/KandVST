#include "EnvelopeComponent.h"



EnvelopeComponent::EnvelopeComponent(int ID):
	IVSTParameters(ID),
	attackLabel(String(), "Attack"),
	decayLabel(String(), "Decay"),
	sustainLabel(String(), "Sustain"),
	releaseLabel(String(), "Release"),
	holdLabel(String(),"Hold"),
	lLabel(String(), "L"),
	tLabel(String(), "T"),
	cLabel(String(), "C")

{
	__envImage = new Image(Image::PixelFormat::RGB, 300, 150, true);
	addAndMakeVisible(attackTime = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_ATTACK_TIME")));
	attackTime->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	attackTime->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 15);
	addAndMakeVisible(holdTime = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_HOLD_TIME")));
	holdTime->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	holdTime->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 15);
	addAndMakeVisible(decayTime = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_DECAY_TIME")));
	decayTime->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	decayTime->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 15);
	addAndMakeVisible(sustainTime = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_SUSTAIN_TIME")));
	sustainTime->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	sustainTime->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 15);
	addAndMakeVisible(releaseTime = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_RELEASE_TIME")));
	releaseTime->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	releaseTime->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 15);
	
	addAndMakeVisible(attackCurve = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_ATTACK_CURVE")));
	attackCurve->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	attackCurve->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 15);
	addAndMakeVisible(decayCurve = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_DECAY_CURVE")));
	decayCurve->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	decayCurve->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 15);
	addAndMakeVisible(sustainCurve = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_SUSTAIN_CURVE")));
	sustainCurve->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	sustainCurve->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 15);
	addAndMakeVisible(releaseCurve = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_RELEASE_CURVE")));
	releaseCurve->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	releaseCurve->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 15);

	addAndMakeVisible(attackLevel = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_ATTACK_LEVEL")));
	attackLevel->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	attackLevel->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 15);
	addAndMakeVisible(decayLevel = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_DECAY_LEVEL")));
	decayLevel->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	decayLevel->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 15);
	addAndMakeVisible(sustainLevel = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_SUSTAIN_LEVEL")));
	sustainLevel->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	sustainLevel->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 15);
	setLookAndFeel(&ourLookAndFeel);
	
	// add some labels for the sliders..
	
	attackLabel.setFont(Font(11.0f));
	attackLabel.setJustificationType(Justification::centred);
	decayLabel.setFont(Font(11.0f));
	decayLabel.setJustificationType(Justification::centred);
	sustainLabel.setFont(Font(11.0f));
	sustainLabel.setJustificationType(Justification::centred);
	releaseLabel.setFont(Font(11.0f));
	releaseLabel.setJustificationType(Justification::centred);
	holdLabel.setFont(Font(11.0f));
	holdLabel.setJustificationType(Justification::centred);

	lLabel.setFont(Font(11.0f));
	lLabel.setJustificationType(Justification::centred);

	tLabel.setFont(Font(11.0f));
	tLabel.setJustificationType(Justification::centred);

	cLabel.setFont(Font(11.0f));
	cLabel.setJustificationType(Justification::centred);


	addAndMakeVisible(attackLabel);
	addAndMakeVisible(decayLabel);
	addAndMakeVisible(sustainLabel);
	addAndMakeVisible(releaseLabel);
	addAndMakeVisible(holdLabel);

	addAndMakeVisible(tLabel);
	addAndMakeVisible(lLabel);
	addAndMakeVisible(cLabel);

	setSize(400, 100);
	addAndMakeVisible(__envImageComponent);
	__envImageComponent.setImage(*__envImage);
	startTimer(50);
}


EnvelopeComponent::~EnvelopeComponent()
{
	delete __envImage;
	__envImage = nullptr;
}

void EnvelopeComponent::resized()
{
	

	Rectangle<int> bounds(getLocalBounds());
	
	__envImageComponent.setBounds(bounds.removeFromTop(jmin<int>(bounds.getWidth(),bounds.getHeight()/2)));
	bounds.removeFromTop(32);

	Rectangle<int> labels(bounds.removeFromTop(20));
	int knobWidth = jmin<int>((bounds.getHeight()-20)/3, bounds.getWidth() / 5);
	labels.removeFromLeft(20);
	attackLabel.setBounds(labels.removeFromLeft(knobWidth));
	holdLabel.setBounds(labels.removeFromLeft(knobWidth));
	decayLabel.setBounds(labels.removeFromLeft(knobWidth));
	sustainLabel.setBounds(labels.removeFromLeft(knobWidth));
	releaseLabel.setBounds(labels.removeFromLeft(knobWidth));
	Rectangle<int> leftLabels(bounds.removeFromLeft(20));
	tLabel.setBounds(leftLabels.removeFromTop(knobWidth));
	cLabel.setBounds(leftLabels.removeFromTop(knobWidth));
	lLabel.setBounds(leftLabels.removeFromTop(knobWidth));

	Rectangle<int> attackCol(bounds.removeFromLeft(knobWidth));
	attackTime->setBounds(attackCol.removeFromTop(knobWidth).reduced(4));
	attackCurve->setBounds(attackCol.removeFromTop(knobWidth).reduced(4));
	attackLevel->setBounds(attackCol.removeFromTop(knobWidth).reduced(4));

	Rectangle<int> holdCol(bounds.removeFromLeft(knobWidth));
	holdTime->setBounds(holdCol.removeFromTop(knobWidth).reduced(4));

	Rectangle<int> decayCol(bounds.removeFromLeft(knobWidth));
	decayTime->setBounds(decayCol.removeFromTop(knobWidth).reduced(4));
	decayCurve->setBounds(decayCol.removeFromTop(knobWidth).reduced(4));
	decayLevel->setBounds(decayCol.removeFromTop(knobWidth).reduced(4));

	Rectangle<int> sustainCol(bounds.removeFromLeft(knobWidth));
	sustainTime->setBounds(sustainCol.removeFromTop(knobWidth).reduced(4));
	sustainCurve->setBounds(sustainCol.removeFromTop(knobWidth).reduced(4));
	sustainLevel->setBounds(sustainCol.removeFromTop(knobWidth).reduced(4));

	Rectangle<int> releaseCol(bounds.removeFromLeft(knobWidth));
	releaseTime->setBounds(releaseCol.removeFromTop(knobWidth).reduced(4));
	releaseCurve->setBounds(releaseCol.removeFromTop(knobWidth).reduced(4));

	delete __envImage;
	__envImage = nullptr;
	__envImage = new Image(Image::PixelFormat::RGB, __envImageComponent.getWidth(), __envImageComponent.getHeight(), true);
	EnvelopeGenerator::RenderImage(__ID, __envImage);
	__envImageComponent.setImage(*__envImage);
	__envImageComponent.repaint();
}
void EnvelopeComponent::timerCallback()
{
	EnvelopeGenerator::RenderImage(__ID, __envImage);
	__envImageComponent.repaint();
}