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

#include "../JuceLibraryCode/JuceHeader.h"
#include "LFOComponent.h"

LFOComponent::~LFOComponent()
{
}

LFOComponent::LFOComponent(int ID,GLOBAL * global):
IVSTParameters(ID)
{
	Global=global;
	__ID = ID;
	int boxWidth = 50;
	__ratioLabel.setText("RATIO", NotificationType::dontSendNotification);
	addAndMakeVisible(__ratioLabel);
	__ratioLabel.setJustificationType(Justification::centred);

	addAndMakeVisible(__type = new ParameterSlider(*Global->paramHandler->Get<AudioParameterInt>(__ID, "LFO_TYPE"), Global));
	__type->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__type->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, boxWidth, 15);
	__type->setDrawProgress(ParameterSlider::ProgressStart::SingleDot);
	__typeLabel.setText("TYPE", NotificationType::dontSendNotification);
	__typeLabel.attachToComponent(__type, false);
	__typeLabel.setJustificationType(Justification::centred);

	addAndMakeVisible(__amp = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "LFO_AMOUNT"), Global));
	__amp->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__amp->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, boxWidth, 15);
	__ampLabel.setText("GAIN", NotificationType::dontSendNotification);
	__ampLabel.attachToComponent(__amp, false);
	__ampLabel.setJustificationType(Justification::centred);

	addAndMakeVisible(__toggleLfo = new ParameterButton(*Global->paramHandler->Get<AudioParameterBool>(__ID, "LFO_EN")));
	__toggleLfo->setButtonText("LFO " + std::to_string(__ID+1));

	addAndMakeVisible(__toggleInvert = new ParameterButton(*Global->paramHandler->Get<AudioParameterBool>(__ID, "LFO_INV")));
	__toggleInvert->setButtonText("INVERT");

	addAndMakeVisible(__onPress = new ParameterButton(*Global->paramHandler->Get<AudioParameterBool>(__ID, "LFO_PRESS")));
	__onPress->setButtonText("ON PRESS");

	__choiceParameter = Global->paramHandler->Get<AudioParameterChoice>(__ID, "LFO_CHOICE");

	addAndMakeVisible(__cBox);
	__cBox.addListener(this);
	__cBox.setColour(ComboBox::backgroundColourId, Swatch::background);
	__cBox.setColour(ComboBox::buttonColourId, Swatch::white);
	
	__cBox.addItemList((*__choiceParameter).choices, 1);
	__cBox.setSelectedItemIndex(__choiceParameter->getIndex(), true);

	
	//setSize(260, 140);
}

void LFOComponent::paint(Graphics& g){
	g.setColour(Swatch::outlineColour);

	int width = getLocalBounds().getWidth();
	int height = getLocalBounds().getHeight();
	int fontHeight = __bounds.getHeight() * 0.2;
	g.drawRect(Rectangle<int>(0, 0, width, height), 2.0f);
	g.fillRect(Rectangle<int>(0, 0, width, fontHeight));


	if (!*Global->paramHandler->Get<AudioParameterBool>(__ID, "LFO_EN")) { //if disabled
		__type->setEnabled(false);
		__amp->setEnabled(false);
		__toggleInvert->setEnabled(false);
		__onPress->setEnabled(false);

		__ratioLabel.setColour(Label::textColourId, Swatch::disabled);
		__typeLabel.setColour(Label::textColourId, Swatch::disabled);
		__ampLabel.setColour(Label::textColourId, Swatch::disabled);
	}
	else { //if enabled
		__type->setEnabled(true);
		__amp->setEnabled(true);
		__toggleInvert->setEnabled(true);
		__onPress->setEnabled(true);

		__ratioLabel.setColour(Label::textColourId, Colours::white);
		__typeLabel.setColour(Label::textColourId, Colours::white);
		__ampLabel.setColour(Label::textColourId, Colours::white);
	}

}

void LFOComponent::resized(){
	__bounds = getLocalBounds();
	
	int fontHeight = __bounds.getHeight() * 0.2;
	Rectangle<int> lfoButtonBounds(0,0, __bounds.getWidth(), fontHeight);
	__toggleLfo->setBounds(lfoButtonBounds);

	Rectangle<int> sliders(__bounds.reduced(8));
	int size = sliders.getWidth() / 4;
	sliders.removeFromTop(fontHeight);
	sliders.removeFromTop(__ampLabel.getFont().getHeight());
	//__ratio->setBounds(sliders.removeFromLeft(size));
	int heightCombo = sliders.getHeight();
	__ratioLabel.setBounds(__bounds.removeFromLeft(__bounds.getWidth() * 0.33).removeFromTop(fontHeight*2.9));
	__cBox.setBounds(sliders.removeFromLeft(size).removeFromBottom(heightCombo * 0.75).removeFromTop(heightCombo * 0.5));
	__type->setBounds(sliders.removeFromLeft(size));
	__amp->setBounds(sliders.removeFromLeft(size));
	__toggleInvert->setBounds(sliders.removeFromTop(20));
	sliders.removeFromTop(20);
	__onPress->setBounds(sliders.removeFromTop(20));

}

void LFOComponent::comboBoxChanged(ComboBox * cbox)
{
	*__choiceParameter = cbox->getSelectedItemIndex();
}
