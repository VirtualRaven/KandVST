#ifndef PARAMETER_BUTTON_H
#define PARAMETER_BUTTON_H
#include <mutex>
#include "../JuceLibraryCode/JuceHeader.h"
class ParameterButton : public ToggleButton, private Button::Listener, private Timer
{
public:
	ParameterButton(AudioProcessorParameter& p);
	~ParameterButton();

	void buttonClicked(Button* button) override;
	int getValue();
	void setValue(int value);
	AudioProcessorParameter& param;
	std::mutex lock;
private:
	int __value;
	// Inherited via Timer
	virtual void timerCallback() override;
	// 1 = on, 0 = off
};
#endif
