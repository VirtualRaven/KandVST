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
#include "MasterPageComponent.h"

MasterComponent::~MasterComponent()
{
}

MasterComponent::MasterComponent(GLOBAL*global) :
	__infoComponent(global),
	__lfoComponent1(0, global),
	__lfoComponent2(1, global),
	__filterComponent(-1, global),
	__settingsComponent(global),
	__delayComponent(global),
	__mixerComponent(global),
	__reverbComponent(-1, global)
{
	Global = global;
	addAndMakeVisible(__infoComponent);
	addAndMakeVisible(__mixerComponent);
	addAndMakeVisible(__filterComponent);
	addAndMakeVisible(__lfoComponent1);
	addAndMakeVisible(__lfoComponent2);

	addAndMakeVisible(__settingsComponent);
	addAndMakeVisible(__delayComponent);
	addAndMakeVisible(__reverbComponent);
}

void MasterComponent::paint(Graphics& g){
	g.setColour(Swatch::background);
	g.fillAll();
}

void MasterComponent::resized() {
	Rectangle<int> bounds = getLocalBounds().reduced(8);

	__mixerComponent.setBounds(bounds.removeFromBottom(260));
	bounds.removeFromBottom(8);
	Rectangle<int> lfoBounds = bounds.removeFromLeft(350);
	int lfoH = (lfoBounds.getHeight() - 8) / 2;
	__lfoComponent1.setBounds(lfoBounds.removeFromTop(lfoH));
	lfoBounds.removeFromTop(8);
	__lfoComponent2.setBounds(lfoBounds.removeFromTop(lfoH));

	Rectangle<int> rightColumnL = bounds.removeFromRight(450);
	Rectangle<int> rightColumnU = rightColumnL.removeFromTop(rightColumnL.getHeight()* 0.5f);
	rightColumnL.removeFromTop(8);
	
	__settingsComponent.setBounds(rightColumnU.removeFromLeft(rightColumnL.getWidth()* 0.6f));
	rightColumnU.removeFromLeft(8);
	__filterComponent.setBounds(rightColumnU);

	__reverbComponent.setBounds(rightColumnL.removeFromLeft(rightColumnL.getWidth()*0.6f));
	rightColumnL.removeFromLeft(8);
	__delayComponent.setBounds(rightColumnL);
	
	bounds.reduce(8, 0);
	__infoComponent.setBounds(bounds);

}