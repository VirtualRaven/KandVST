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

#include "SettingsComponent.h"

SettingsComponent::~SettingsComponent()
{

}

SettingsComponent::SettingsComponent(GLOBAL*global)
{
	Global = global;
	__pitchBendSens = new ParameterSlider(*Global->paramHandler->Get<AudioParameterInt>(-1, "PITCH_BEND_SENS"),global);
	__selectKnob = new ParameterSlider(*Global->paramHandler->Get<AudioParameterChoice>(-1, "SELECT_KNOB_VALUE"),global);
	__selectKnob->setSize(50, 50);

	addAndMakeVisible(__pitchBendSens);
	addAndMakeVisible(__selectKnob);

	__pitchBendSens->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__pitchBendSens->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 50, 15);
	__pitchBendLabel.setText("PB Sens", NotificationType::dontSendNotification);
	__pitchBendLabel.attachToComponent(__pitchBendSens, false);
	__pitchBendLabel.setJustificationType(juce::Justification::centred);

}

void SettingsComponent::paint(Graphics& g)
{
	int width = getLocalBounds().getWidth();
	int height = getLocalBounds().getHeight();
	int fontHeight = getLocalBounds().getHeight() * 0.2;
	int fontSize = getLocalBounds().getHeight() * 0.14;

	g.setColour(Colour::fromRGB(60, 60, 60));
	g.drawRect(Rectangle<int>(0, 0, width, height), 2.0f);
	g.fillRect(Rectangle<int>(0, 0, width, fontHeight));

	g.setColour(Colours::white);
	Font filter(fontSize, Font::FontStyleFlags::bold);
	g.setFont(filter);
	g.drawText("HARDWARE SETTINGS", Rectangle<int>(0, 0, width, fontHeight), Justification::centred, false);
	
}

void SettingsComponent::resized() 
{
	
	__bounds = getLocalBounds();
	int fontHeight = __bounds.getHeight() * 0.2;
	Rectangle<int> settings(__bounds.reduced(8));
	settings.removeFromTop(fontHeight);
	settings.removeFromTop(__pitchBendLabel.getFont().getHeight());
	
	int space = settings.getWidth() * 0.5;
	
	__pitchBendSens->setBounds(settings.removeFromLeft(space));
	
	__selectKnob->setBounds(settings.removeFromLeft(2*space));

}

void SettingsComponent::RegisterParameters(int ID,GLOBAL*Global)
{
	Global->paramHandler->RegisterInt(ID, "PITCH_BEND_SENS", "Pitch bend sensitivity", 1, 12, 2);
        auto tmp = StringArray("1", "2", "3", "4", "5");
	Global->paramHandler->RegisterChoice(ID, "SELECT_KNOB_VALUE", "Select Knob",tmp , 1);
}
