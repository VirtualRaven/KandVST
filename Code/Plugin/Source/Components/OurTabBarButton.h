#ifndef OUR_TAB_BAR_BUTTON_H
#define OUR_TAB_BAR_BUTTON_H

#include "../JuceLibraryCode/JuceHeader.h"

class OurTabBarButton : public TabBarButton 
{
public:
	void paintButton(Graphics&, bool isMouseOverButton, bool isButtonDown) override;


private:
	TabbedButtonBar & __owner;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OurTabBarButton)

};

#endif OUR_TAB_BAR_BUTTON_H