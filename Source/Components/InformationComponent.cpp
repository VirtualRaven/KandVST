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
#include "InformationComponent.h"

InformationComponent::~InformationComponent()
{
}

InformationComponent::InformationComponent(GLOBAL*global):
__presetComponent(global)
{
	Global = global;
	addAndMakeVisible(__presetComponent);

	// variables
	x = 310; //starting x position
	y = 20; //starting y position
	midX = 500;
	midY = 225;
	w = 380; //width of component
	h = 250; //height of component

	// shadow stuff
	startAlpha = 0.3f;
	endAlpha = 0.05f;
	offset = 10;
}


void InformationComponent::paint (Graphics& g){
    
    //Path to draw the rectangle
    Path myPath;
    Rectangle<int> infoWindow = getLocalBounds();
	x = infoWindow.getX();
	y = infoWindow.getY();
	midX = infoWindow.getCentreX();
	midY = infoWindow.getCentreY();
	w = infoWindow.getWidth();
	h = infoWindow.getHeight();
	myPath.addRectangle(infoWindow);

	ThemePicker tp;

	Colour c = tp.getColour(Global->paramHandler->Get<AudioParameterChoice>(-1, "THEME")->getIndex());

	g.setColour(c.darker(1.f).darker(0.7f).withSaturation(0.2f));

    //g.setColour(Colour::fromRGB(40,60,90));
    g.fillPath(myPath);

	g.setColour(Colours::white);
	g.setFont(Font (20, Font::bold));
	g.drawText("PRESETS", Rectangle<int>(x + 15, y + offset * 2, w - offset * 3, 30.0f),
		Justification::centred, false);

    // == adding inner shadows using gradients == 
    //shadow for top
    ColourGradient shadowTop(
		Colours::black.withAlpha(startAlpha), midX, y, Colours::black.withAlpha(endAlpha), midX, y+offset, false);
                    
    g.setGradientFill(shadowTop);
    g.fillPath(myPath);

    //shadow for bottom
    ColourGradient shadowBottom(
		Colours::black.withAlpha(startAlpha), midX, y+h, Colours::black.withAlpha(endAlpha), midX, ((y+h)-offset) , false);
    g.setGradientFill(shadowBottom);
    g.fillPath(myPath);

    //shadow for left
    ColourGradient shadowLeft(
		Colours::black.withAlpha(startAlpha), x, midY, Colours::black.withAlpha(endAlpha), x+offset, midY, false);
    g.setGradientFill(shadowLeft);
    g.fillPath(myPath);

    //shadow for right
    ColourGradient shadowRight(
		Colours::black.withAlpha(startAlpha), x+w, midY, Colours::black.withAlpha(endAlpha), ((x+w)-offset), midY, false);
    g.setGradientFill(shadowRight);
    g.fillPath(myPath);


    g.setColour(Colours::black);
    g.strokePath(myPath,PathStrokeType (0.8f));

	g.setColour(Colours::white);
	g.drawRect(__presetComponent.getBounds().reduced(6), 1.2f);
	g.drawRect(Rectangle<int>(x + 15, y + offset * 2, w - offset * 3, 30.0f), 1.2f);
	

}
void InformationComponent::resized(){
	Rectangle<int> bounds = getLocalBounds();
	bounds.removeFromTop(offset+30);
	bounds.removeFromTop(8);

	__presetComponent.setBounds(bounds.reduced(9));
}
