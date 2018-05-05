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

#include "MixerSubComponent.h"

MixerSubComponent::~MixerSubComponent()
{
}


MixerSubComponent::MixerSubComponent(int ID, String  label,GLOBAL*global):
	MixerSubComponent(ID, label, "OSC_PAN", "OSC_MIX_AMP", false,global)
{

}

MixerSubComponent::MixerSubComponent(int ID, String label, String overridePanId, String overrideGainId, bool isMaster,GLOBAL*global) :
	IVSTParameters(ID),
	__isMaster(isMaster)
{
	Global=global;
	__gain = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(ID, overrideGainId),Global);
	
	if (!__isMaster)
		__pan = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(ID, overridePanId), Global);
	


	//===========================
	// ADD AND MAKE VISIBLE

	addAndMakeVisible(__gain);
	
	if (!__isMaster)
	{
		addAndMakeVisible(__pan);
		addAndMakeVisible(__panR);
		addAndMakeVisible(__panL);
	}


	//==========================

	__gain->setSliderStyle(Slider::LinearVertical);
	__gain->setTextBoxStyle(__gain->NoTextBox, true, 55, 15);
	__label.setText(label, NotificationType::dontSendNotification);
	__label.attachToComponent(__gain, false);
	__label.setJustificationType(Justification::centred);


	//====================================
	//Pans
	if (!__isMaster)
	{
		__pan->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
		__pan->setTextBoxStyle(__pan->NoTextBox, true, 10, 10);
		__pan->setDrawProgress(ParameterSlider::ProgressStart::Center);
		//====================================
		// Pan labels
		__panL.setText("L", NotificationType::dontSendNotification);
		__panL.setJustificationType(juce::Justification::topLeft);
		//__panL.setColour(Label::ColourIds::backgroundColourId, Colours::black);
		__panL.setBorderSize(BorderSize<int>(0));
		__panR.setText("R", NotificationType::dontSendNotification);
		__panR.setJustificationType(juce::Justification::topLeft);
		//__panR.setColour(Label::ColourIds::backgroundColourId, Colours::black);
		__panR.setBorderSize(BorderSize<int>(0));
	}
	//=====================================
}

void MixerSubComponent::paint(Graphics& g) {


}

void MixerSubComponent::resized() {
	
	__bounds = getLocalBounds();
	if (!__isMaster)
	{
		__bounds.removeFromLeft(__panL.getFont().getStringWidth("L"));
		__bounds.removeFromRight(__panR.getFont().getStringWidth("R"));
		int panSize = jmin<int>(40, __bounds.getWidth());
		int panPadd = jmax<int>(0, (__bounds.getWidth() - panSize) / 2);
		__pan->setBounds(__bounds.removeFromTop(panSize).removeFromLeft(panPadd + panSize).removeFromRight(panSize));
		__bounds.removeFromTop(24);

		Rectangle<int> ml(__pan->getBounds());
		__panL.setBounds(__pan->getBounds().getX() - __panL.getFont().getStringWidth("L"), ml.getY(), __panL.getFont().getStringWidth("L"), __panL.getFont().getHeight());
		__panR.setBounds(__pan->getBounds().getX() + panSize, ml.getY(), __panR.getFont().getStringWidth("R"), __panR.getFont().getHeight());
	}
	else
	{
		__bounds.removeFromTop(20);
	}

	__gain->setBounds(__bounds);	
}