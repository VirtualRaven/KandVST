#ifndef THEME_PICKER_H
#define THEME_PICKER_H

#include "Swatch.h"

class ThemePicker {
public:
	ThemePicker();
	~ThemePicker();

	Colour getColour(int themeId);
	Colour getColour(String themeName);
private:

};

#endif THEME_PICKER_H
