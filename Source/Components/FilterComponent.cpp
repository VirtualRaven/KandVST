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

#include "FilterComponent.h"

FilterComponent::~FilterComponent()
{

}
FilterComponent::FilterComponent(int ID,GLOBAL*global)
{
	Global = global;
	__hpFilter = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(ID, "FILTER_HP"), Global);
	__lpFilter = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(ID, "FILTER_LP"), Global);

	addAndMakeVisible(__hpFilter);
	addAndMakeVisible(__lpFilter);

	__lpFilter->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__lpFilter->setTextBoxStyle(__lpFilter->NoTextBox, true, 10, 10);
	__lpFilter->setSkewFactorFromMidPoint(0.15);
	__lpFilter->setDrawProgress(ParameterSlider::ProgressStart::End);
	__lpLabel.setText("LP", NotificationType::dontSendNotification);
	__lpLabel.attachToComponent(__lpFilter, false);
	__lpLabel.setJustificationType(juce::Justification::horizontallyCentred);

	__hpFilter->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__hpFilter->setTextBoxStyle(__hpFilter->NoTextBox, true, 10, 10);
	__hpFilter->setSkewFactorFromMidPoint(0.15);
	__hpFilter->setDrawProgress();

	__hpLabel.setText("HP", NotificationType::dontSendNotification);
	__hpLabel.attachToComponent(__hpFilter, false);
	__hpLabel.setJustificationType(juce::Justification::horizontallyCentred);

}

void FilterComponent::paint(Graphics& g) {

	int width = __bounds.getWidth();
	int height = __bounds.getHeight();
	int fontHeight = __bounds.getHeight() * 0.2;
	int fontSize = __bounds.getHeight() * 0.16;

	g.setColour(Colour::fromRGB(60, 60, 60));
	g.drawRect(Rectangle<float>(0, 0, width, height), 2.0f);
	g.fillRect(Rectangle<float>(0, 0, width, fontHeight));
	
	g.setColour(Colours::white);
	Font filter(fontSize, Font::FontStyleFlags::bold);
	g.setFont(filter);
	g.drawText("FILTER", Rectangle<int>(0, 0, width, fontHeight), Justification::centred, false);
	
}

void FilterComponent::resized() {

	__bounds = getLocalBounds();

	Rectangle<int> temp = __bounds.reduced(8);
	__lpLabel.setFont(Font(__bounds.getHeight() * 0.13, Font::plain));
	__hpLabel.setFont(Font(__bounds.getHeight() * 0.13, Font::plain));
	int fontHeight = __bounds.getHeight() * 0.2;
	temp.removeFromTop(fontHeight);
	temp.removeFromTop(__hpLabel.getFont().getHeight());
	int size = temp.getWidth() / 2;
	__lpFilter->setBounds(temp.removeFromRight(size));
	__hpFilter->setBounds(temp.removeFromRight(size));

}