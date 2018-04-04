#ifndef ABOUT_PAGE_COMPONENT_H
#define ABOUT_PAGE_COMPONENT_H

#include "JuceHeader.h"
#include "Global.h"
#include "OurLookAndFeel.h"

class AboutPageComponent : public Component{
public :
	AboutPageComponent(GLOBAL * global);
	~AboutPageComponent();

	void paint(Graphics& g) override;
	void resized() override;
private:
	GLOBAL * Global;
	TextEditor __info;
	ComboBox __themes;
	OurLookAndFeel ourLookAndFeel;
	String __about;
};



#endif ABOUT_PAGE_COMPONENT_H