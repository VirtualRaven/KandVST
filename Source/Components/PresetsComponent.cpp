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
#include "PresetsComponent.h"

PresetsComponent::~PresetsComponent()
{
}

PresetsComponent::PresetsComponent(GLOBAL*global):
__rowModel(&__presets,global)
{ 
	Global = global;
	__refresh.addListener(this);
	Image refImage = ImageFileFormat::loadFrom(Resources::Icons::refresh_png, sizeof(Resources::Icons::refresh_png));
	__refresh.setImages(false, true, true, refImage, 1.0f, Colours::transparentBlack, refImage, 0.7f, Colours::transparentBlack, refImage, 0.5f, Colours::transparentBlack);
	addAndMakeVisible(__presets);
	addAndMakeVisible(__refresh);
	//addAndMakeVisible(__delete);
	//addAndMakeVisible(__save);

	setSize(200, 200);
	__presets.setModel(&__rowModel);
}
void PresetsComponent::resized()
{
	Rectangle<int> localBounds(getLocalBounds().reduced(8));
	__presets.setBounds(localBounds);
	Rectangle<int> refBtn(localBounds);
	__refresh.setBounds(refBtn.removeFromBottom(25).removeFromRight(25));
	
}

void PresetsComponent::buttonClicked(Button * btn)
{
	if (btn == &__refresh)
	{
		Global->presetManager->RefreshPresets();
		__presets.updateContent();
	}
}