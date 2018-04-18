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