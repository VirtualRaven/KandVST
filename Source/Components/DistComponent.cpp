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
#include "DistComponent.h"

DistComponent::~DistComponent()
{
}

DistComponent::DistComponent(int ID, GLOBAL * global) :
	IVSTParameters(ID),
	__ID(ID)
{
	Global=global;
	addAndMakeVisible(__threshold = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "DIST_TRSH"), Global));
	__threshold->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__threshold->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 50, 15);


	addAndMakeVisible(__toggleDist = new ParameterButton(*Global->paramHandler->Get<AudioParameterBool>(__ID, "DIST_EN")));
	__toggleDist->setButtonText("Dist " + std::to_string(__ID + 1));
	setSize(140, 140);
}

void DistComponent::paint(Graphics& g) {
	g.setColour(Colour::fromRGB(60, 60, 60));

	int width = __bounds.getWidth();
	int height = __bounds.getHeight();
	int fontHeight = __bounds.getHeight() * 0.2;
	g.drawRect(Rectangle<int>(0, 0, width, height), 2.0f);
	g.fillRect(Rectangle<int>(0, 0, width, fontHeight));

	if (!*Global->paramHandler->Get<AudioParameterBool>(__ID, "DIST_EN")) {
		__threshold->setEnabled(false);
		__toggleDist->setColour(ParameterButton::textColourOffId, Swatch::disabled);
	}
	else {
		__threshold->setEnabled(true);
		__toggleDist->setColour(ParameterButton::textColourOnId, Colours::white);
	}
}

void DistComponent::resized() {
	__bounds = getLocalBounds();

	int fontHeight = __bounds.getHeight() * 0.2;
	Rectangle<int> distButtonBounds(0, 0, __bounds.getWidth(), fontHeight);
	__toggleDist->setBounds(distButtonBounds);

	Rectangle<int> sliders(__bounds.reduced(8));
	int size = sliders.getWidth();
	sliders.removeFromTop(fontHeight);
	__threshold->setBounds(sliders.removeFromLeft(size));
}