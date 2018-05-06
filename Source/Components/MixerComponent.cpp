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

#include "MixerComponent.h"

MixerComponent::~MixerComponent()
{
	for (size_t i = 0; i < __mixers.size(); i++)
	{
		delete __mixers[i];
	}
}


MixerComponent::MixerComponent(GLOBAL* global)
	: __vuComp(global)
{
	Global = global;
	__mixers.push_back(new MixerSubComponent(-1, "MASTER", "MASTER_GAIN", "MASTER_GAIN", true, Global));
	for (size_t i = 0; i < 4; i++)
	{
		__mixers.push_back(new MixerSubComponent(i, String("OSC ")+String(i+1), Global));
	}

	for (auto&& msc : __mixers)
	{
		addAndMakeVisible(msc);
	}

	addAndMakeVisible(__vuComp);
}

void MixerComponent::paint(Graphics& g) {

	int width = __bounds.getWidth();
	int height = __bounds.getHeight();
	int fontHeight = __bounds.getHeight() * 0.1;
	int fontSize = __bounds.getHeight() * 0.08;

	g.setColour(Colour::fromRGB(60, 60, 60));
	g.drawRect(Rectangle<int>(0, 0, width, height), 2.0f);
	g.fillRect(Rectangle<int>(0, 0, width, fontHeight));

	Font mixer(fontSize, Font::FontStyleFlags::bold);
	g.setColour(Colours::white);
	g.setFont(mixer);
	g.drawText("MIXER", Rectangle<int>(0, 0, width, fontHeight), Justification::centred, false);

}

void MixerComponent::resized() {
	__bounds = getLocalBounds();
	int fontHeight = __bounds.getHeight() * 0.1;
	int mixW = jmin<int>(120, __bounds.getWidth() / __mixers.size());
	int gap = jmax<int>(0, (__bounds.getWidth() - mixW * __mixers.size()) / (__mixers.size()+1));
	Rectangle<int> mix = __bounds.reduced(8);
	mix.removeFromTop(fontHeight);
	__vuComp.setBounds(mix.removeFromLeft(gap));
	__mixers[0]->setBounds(mix.removeFromLeft(mixW));
	for (size_t i = 1; i < __mixers.size(); i++)
	{
		mix.removeFromLeft(gap);
		__mixers[i]->setBounds(mix.removeFromLeft(mixW));
	}
}