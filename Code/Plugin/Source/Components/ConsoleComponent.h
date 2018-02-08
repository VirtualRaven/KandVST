#ifndef CONSOLE_COMPONENET_H
#define CONSOLE_COMPONENET_H

#include "../Global.h"
#include "../ParameterHandler.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include <iostream>
#include <stdlib.h>
class ConsoleComponent :public  Component, private KeyListener
{
private:
	TextEditor __input;
	TextEditor __output;
	std::stringstream __buffer;
	std::vector<std::string> split(const std::string& s, char seperator);
	std::vector<std::string> __lastCommands;
	int __lastCounter;
	void setParam(String id, float value);
public:
	ConsoleComponent();
	~ConsoleComponent();
	void resized() override;

	// Inherited via KeyListener
	virtual bool keyPressed(const KeyPress & key, Component * originatingComponent) override;
};

#endif // !CONSOLE_COMPONENET_H