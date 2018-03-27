#ifndef CONSOLE_COMPONENET_H
#define CONSOLE_COMPONENET_H

#include "Global.h"
#include "ParameterHandler.h"
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
	size_t __lastCounter;
	void setParam(String id, std::string value);
public:
	ConsoleComponent(GLOBAL* global);
	~ConsoleComponent();
	void resized() override;
	void paint(Graphics & g) override;
	GLOBAL * Global;
	// Inherited via KeyListener
	virtual bool keyPressed(const KeyPress & key, Component * originatingComponent) override;
};

#endif // !CONSOLE_COMPONENET_H