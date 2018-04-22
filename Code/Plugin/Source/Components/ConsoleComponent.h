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

#ifndef CONSOLE_COMPONENET_H
#define CONSOLE_COMPONENET_H

#include "Global.h"
#include "ParameterHandler.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include <iostream>
#include <stdlib.h>
class ConsoleComponent :public  Component, private KeyListener
{
private:
	TextEditor __input;
	TextEditor __output;
	std::stringstream __buffer;
	std::vector<std::string> split(const std::string& s, char seperator);
	std::vector<std::string> __lastCommands;
	size_t __lastCounter;
	void setParam(String id, std::string value);
public:
	ConsoleComponent(GLOBAL* global);
	~ConsoleComponent();
	void resized() override;
	void paint(Graphics & g) override;
	GLOBAL * Global;
	// Inherited via KeyListener
	virtual bool keyPressed(const KeyPress & key, Component * originatingComponent) override;
};

#endif // !CONSOLE_COMPONENET_H