#ifndef OUR_TABBED_BUTTON_BAR_H
#define OUR_TABBED_BUTTON_BAR_H


#include "../JuceLibraryCode/JuceHeader.h"

class OurTabbedButtonBar;

class OurTabBarButton : public TabBarButton {
public:
	void paintButton(Graphics&, bool isMouseOverButton, bool isButtonDown) override;
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OurTabBarButton)
};

class OurTabbedButtonBar : public TabbedButtonBar
{
public:
	virtual OurTabBarButton* createTabButton(const String& tabName, int tabIndex) override;

private:


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OurTabbedButtonBar)
};

#endif
