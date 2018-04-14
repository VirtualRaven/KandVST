#ifndef SWATCH
#define SWATCH
#include "JuceHeader.h"
namespace Swatch
{
	static const Colour background = Colour::fromRGB(36,36,36);
	static const Colour disabled = Colour::fromRGB(150, 150, 150);
	static const Colour outlineColour = Colour::fromRGB(60, 60, 60);
	static const Colour white = Colours::white;

	//default theme 
	static const Colour accentBlue = Colour::fromRGB(26, 105, 180);

	//pink theme
	static const Colour accentPink = Colour::fromRGB(200, 0, 145);

	//green theme
	static const Colour accentGreen = Colour::fromRGB(0, 200, 50);

}
#endif