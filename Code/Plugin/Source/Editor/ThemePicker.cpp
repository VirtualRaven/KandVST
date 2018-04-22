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

#include "ThemePicker.h"

ThemePicker::~ThemePicker() {
}

ThemePicker::ThemePicker() {

}

Colour ThemePicker::getColour(int themeId) {
	switch (themeId) {
	case 0: return Swatch::accentBlue;
		break;
	case 1: return Swatch::accentPink;
		break;
	case 2: return Swatch::accentGreen;
		break;
	default: return Swatch::accentBlue;
		break;
	}
}

Colour ThemePicker::getColour(String themeName) {

	if (themeName == "Default") {
		return Swatch::accentBlue;
	}
	if (themeName == "Pink") {
		return Swatch::accentPink;
	}
	if (themeName == "Green") {
		return Swatch::accentGreen;
	}
	return Swatch::accentBlue;
}