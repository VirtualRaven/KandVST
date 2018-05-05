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

#ifndef SWATCH
#define SWATCH
#include "JuceHeader.h"
namespace Swatch
{
	static const Colour background = Colour::fromRGB(36,36,36);
	static const Colour disabled = Colour::fromRGB(150, 150, 150);
	static const Colour outlineColour = Colour::fromRGB(60, 60, 60);
	static const Colour white = Colour::fromRGB(255, 255, 255);

	//default theme 
	static const Colour accentBlue = Colour::fromRGB(26, 105, 180);

	//pink theme
	static const Colour accentPink = Colour::fromRGB(200, 0, 145);

	//green theme
	static const Colour accentGreen = Colour::fromRGB(0, 200, 50);

	//orange theme
	static const Colour accentOrange = Colour::fromRGB(255,102,0);

	//red theme
	static const Colour accentRed = Colour::fromRGB(220, 0, 0);

}
#endif