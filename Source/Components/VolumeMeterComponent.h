#ifndef VOLUME_METER_COMPONENT_H
#define VOLUME_METER_COMPONENT_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "Global.h"
#include "Swatch.h"
#include "PluginProcessor.h"

class VolumeMeterComponent : public Component, private Timer {
public:
	VolumeMeterComponent(GLOBAL* global);
	~VolumeMeterComponent();

	void paint(Graphics& g) override;
	void timerCallback() override;

private:
	GLOBAL * Global;
	Rectangle<int> __0, __1, __2, __3, __4, __5, __6;
	std::vector<Rectangle<int>> rects;
	float __db = 0.f;
};
#endif //VOLUME_METER_COMPONENT_H
