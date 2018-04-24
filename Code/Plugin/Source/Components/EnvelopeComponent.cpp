/*
 * This file is part of the KandVST synthesizer.
 *
 * Copyright (c) 2018   Lukas Rahmn, Anton Fredriksson,
 *                      Sarosh Nasir, Stanisław Zwierzchowski,
 *                      Klas Ludvigsson and Andreas Kuszli
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "EnvelopeComponent.h"



EnvelopeComponent::EnvelopeComponent(int ID, GLOBAL * global):
	IVSTParameters(ID),
	attackLabel(String(), "Attack"),
	decayLabel(String(), "Decay"),
	sustainLabel(String(), "Sustain"),
	releaseLabel(String(), "Release"),
	holdLabel(String(),"Hold")
{
	Global=global;
	__envImage = new Image(Image::PixelFormat::RGB, 300, 150, true);
	addAndMakeVisible(attackTime = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_ATTACK_TIME"), Global));
	attackTime->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	attackTime->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 15);
	attackTime->setSkewFactor(0.55);
	addAndMakeVisible(holdTime = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_HOLD_TIME"), Global));
	holdTime->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	holdTime->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 15);
	holdTime->setSkewFactor(0.55);
	addAndMakeVisible(decayTime = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_DECAY_TIME"), Global));
	decayTime->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	decayTime->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 15);
	decayTime->setSkewFactor(0.55);
	addAndMakeVisible(sustainTime = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_SUSTAIN_TIME"), Global));
	sustainTime->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	sustainTime->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 15);
	sustainTime->setSkewFactor(0.55);
	addAndMakeVisible(releaseTime = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_RELEASE_TIME"), Global));
	releaseTime->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	releaseTime->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 15);
	releaseTime->setSkewFactor(0.55);
	
	addAndMakeVisible(attackCurve = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_ATTACK_CURVE"), Global));
	attackCurve->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	attackCurve->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 15);
	attackCurve->setSkewFactor(0.25);
	addAndMakeVisible(decayCurve = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_DECAY_CURVE"), Global));
	decayCurve->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	decayCurve->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 15);
	decayCurve->setSkewFactor(0.25);
	addAndMakeVisible(sustainCurve = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_SUSTAIN_CURVE"), Global));
	sustainCurve->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	sustainCurve->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 15);
	sustainCurve->setSkewFactor(0.25);
	addAndMakeVisible(releaseCurve = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_RELEASE_CURVE"), Global));
	releaseCurve->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	releaseCurve->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 15);
	releaseCurve->setSkewFactor(0.25);

	addAndMakeVisible(attackLevel = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_ATTACK_LEVEL"), Global));
	attackLevel->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	attackLevel->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 15);
	addAndMakeVisible(decayLevel = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_DECAY_LEVEL"), Global));
	decayLevel->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	decayLevel->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 15);
	addAndMakeVisible(sustainLevel = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_SUSTAIN_LEVEL"), Global));
	sustainLevel->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	sustainLevel->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 15);

	
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

	addAndMakeVisible(attackLabel);
	addAndMakeVisible(decayLabel);
	addAndMakeVisible(sustainLabel);
	addAndMakeVisible(releaseLabel);
	addAndMakeVisible(holdLabel);

	setSize(400, 100);
	addAndMakeVisible(__envImageComponent);
	__envImageComponent.setImage(*__envImage);
	EnvelopeGenerator::RenderImage(__ID, __envImage,Global);
	__envImageComponent.repaint();
	Global->paramHandler->addParamaterListener(this, ID, "ENV_ATTACK_TIME");
	Global->paramHandler->addParamaterListener(this, ID, "ENV_HOLD_TIME");
	Global->paramHandler->addParamaterListener(this, ID, "ENV_DECAY_TIME");
	Global->paramHandler->addParamaterListener(this, ID, "ENV_SUSTAIN_TIME");
	Global->paramHandler->addParamaterListener(this, ID, "ENV_RELEASE_TIME");
	Global->paramHandler->addParamaterListener(this, ID, "ENV_ATTACK_CURVE");
	Global->paramHandler->addParamaterListener(this, ID, "ENV_DECAY_CURVE");
	Global->paramHandler->addParamaterListener(this, ID, "ENV_SUSTAIN_CURVE");
	Global->paramHandler->addParamaterListener(this, ID, "ENV_RELEASE_CURVE");
	Global->paramHandler->addParamaterListener(this, ID, "ENV_ATTACK_LEVEL");
	Global->paramHandler->addParamaterListener(this, ID, "ENV_DECAY_LEVEL");
	Global->paramHandler->addParamaterListener(this, ID, "ENV_SUSTAIN_LEVEL");
	this->addComponentListener(this);
}


EnvelopeComponent::~EnvelopeComponent()
{
	Global->paramHandler->removeParamaterListener(this);
	delete __envImage;
	__envImage = nullptr;
}

void EnvelopeComponent::paint(Graphics & g) {
	
	g.setColour(Colour::fromRGB(60, 60, 60));
	int startY = getLocalBounds().getHeight() / 2 + 13;
	int startX = 24;
	int height = (getLocalBounds().getHeight() / 2) * 0.88;
	
	for (int i = 0; i < 5; i++) {
		g.fillRect(Rectangle<int>(startX, startY, 60, 15));
		startX += 69;
	}

	Rectangle<int> bounds(getLocalBounds().reduced(8));
	g.drawRect(Rectangle<int>(18, startY + 7.5, bounds.getWidth()-2, height));

	//=============================================================================
	g.saveState();

	g.setColour(Colours::white);
	g.setFont(Font(12, Font::bold));
	int translateX = -250;
	int translateY = 515;
	float angle = -float_Pi / 2.0f;
	g.addTransform(AffineTransform::identity.rotated(angle).followedBy(AffineTransform::identity.translated(translateX, translateY)));
	
	Rectangle<int> sideLabels(getLocalBounds().removeFromLeft(500));
	g.drawText("TIME", 
		Rectangle<int>(sideLabels.getX()+8, sideLabels.getY(), sideLabels.getWidth(), sideLabels.getHeight()),
		Justification::centred, false);

	sideLabels.removeFromRight(125);
	g.drawText("CURVE",
		Rectangle<int>(sideLabels.getX(), sideLabels.getY(), sideLabels.getWidth(), sideLabels.getHeight()),
		Justification::centred, false);
	
	sideLabels.removeFromRight(130);
	g.drawText("LEVEL",
		Rectangle<int>(sideLabels.getX(), sideLabels.getY(), sideLabels.getWidth(), sideLabels.getHeight()),
		Justification::centred, false);

	g.restoreState();
	//=============================================================================
}

void EnvelopeComponent::resized()
{
	

	Rectangle<int> bounds(getLocalBounds());
	
	__envImageComponent.setBounds(bounds.removeFromTop(jmin<int>(bounds.getWidth(),bounds.getHeight()/2)));
	bounds.removeFromTop(10);

	Rectangle<int> labels(bounds.removeFromTop(20));
	int knobWidth = jmin<int>((bounds.getHeight()-20)/3, bounds.getWidth() / 5);
	labels.removeFromLeft(19);
	attackLabel.setBounds(labels.removeFromLeft(knobWidth));
	holdLabel.setBounds(labels.removeFromLeft(knobWidth));
	decayLabel.setBounds(labels.removeFromLeft(knobWidth));
	sustainLabel.setBounds(labels.removeFromLeft(knobWidth));
	releaseLabel.setBounds(labels.removeFromLeft(knobWidth));
	Rectangle<int> leftLabels(bounds.removeFromLeft(20));

/*
	tLabel.setBounds(leftLabels.removeFromTop(knobWidth));
	cLabel.setBounds(leftLabels.removeFromTop(knobWidth));
	lLabel.setBounds(leftLabels.removeFromTop(knobWidth));
*/
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
	__envImage = new Image(Image::PixelFormat::RGB, __envImageComponent.getWidth()*2, __envImageComponent.getHeight()*2, true);
	EnvelopeGenerator::RenderImage(__ID, __envImage,Global);
	__envImageComponent.setImage(*__envImage);
	__envImageComponent.repaint();
}
void EnvelopeComponent::parametersChanged(std::vector<std::string>)
{
	__envInvalid = true;
	if (!isTimerRunning())
		startTimerHz(120);
	
}
void EnvelopeComponent::timerCallback()
{
	if (!this->getParentComponent()->isVisible()) {
		stopTimer();
		return;
	}
	if (!__envInvalid) {
		stopTimer();
		return;
	}
		EnvelopeGenerator::RenderImage(__ID, __envImage,Global);
		__envImageComponent.repaint();
		__envInvalid = false;

}

void EnvelopeComponent::componentVisibilityChanged(Component & component)
{
	if (__envInvalid && component.isVisible()) {
		EnvelopeGenerator::RenderImage(__ID, __envImage,Global);
		__envImageComponent.repaint();
		__envInvalid = false;
	}

}

void EnvelopeComponent::componentParentHierarchyChanged(Component & component)
{
	this->getParentComponent()->addComponentListener(this);
	this->removeComponentListener(this);
}

void EnvelopeComponent::lookAndFeelChanged() {
	EnvelopeGenerator::RenderImage(__ID, __envImage, Global);
	__envImageComponent.repaint();
	__envInvalid = false;
}
