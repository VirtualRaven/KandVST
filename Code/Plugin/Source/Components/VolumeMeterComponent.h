#ifndef VOLUME_METER_COMPONENT_H
#define VOLUME_METER_COMPONENT_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "Global.h"
class VolumeMeterComponent : public Component, public Timer {
public:
	VolumeMeterComponent(GLOBAL* global);
	~VolumeMeterComponent();

	void paint(Graphics& g) override;
	void resized() override;

private:
	GLOBAL * Global;
	Rectangle<int> __01, __02, __11, __12, __21, __22, __31, __32;
	std::vector<Rectangle<int>> rects;


	// Inherited from Timer
	void timerCallback() override;

};

#endif VOLUME_METER_COMPONENT_H