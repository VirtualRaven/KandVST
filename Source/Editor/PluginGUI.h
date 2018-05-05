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

#ifndef PLUGIN_GUI_H
#define PLUGIN_GUI_H

#include "PluginProcessor.h"
#include "../JuceLibraryCode/JuceHeader.h"

#include "Components/ConsoleComponent.h"
#include "Components/MasterPageComponent.h"
#include "Components/OscillatorPageComponent.h"
#include "Components/AboutPageComponent.h"
#include "OurLookAndFeel.h"

class PluginGUI  : public AudioProcessorEditor, private KeyListener, private Timer            
{
public:
	PluginGUI(PluginProcessor& owner,GLOBAL* global);
	virtual ~PluginGUI();

	void paint(Graphics& g);
	void resized();
	void InitializeGui();
	GLOBAL * Global;

private:
	OurLookAndFeel ourLookAndFeel; //Must be on top
	ImageComponent  __loadingImage;
	TabbedComponent __tabComponent;
	MidiKeyboardComponent __keyboard;
#ifdef CONSOLE
	ConsoleComponent __cc;
#endif

	PluginProcessor* __owner;
	bool __initialized;

    //==============================================================================
	PluginProcessor& getProcessor() const
    {
        return static_cast<PluginProcessor&> (processor);
    }

	void updateTimecodeDisplay(AudioPlayHead::CurrentPositionInfo pos);

	// Inherited via KeyListener
	virtual bool keyPressed(const KeyPress & key, Component * originatingComponent) override;
	virtual bool keyStateChanged(bool isKeyDown, Component* originatingComponent);


	// Inherited via Timer
	virtual void timerCallback() override;
	bool __guiInit;
};
#endif // !PLUGIN_GUI_H
