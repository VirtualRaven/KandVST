#include "OurTabbedButtonBar.h"

void OurTabBarButton::paintButton(Graphics & g, bool isMouseOverButton, bool isButtonDown)
{
	getLookAndFeel().drawTabButton(this*, g, isMouseOverButton, isButtonDown);
}

//====================================================
OurTabBarButton * OurTabbedButtonBar::createTabButton(const String & tabName, int tabIndex)
{
	return nullptr;
}

