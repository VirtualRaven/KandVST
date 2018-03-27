#include "ConsoleComponent.h"



ConsoleComponent::ConsoleComponent(GLOBAL * global)
{
	Global = global;
	__input.setMultiLine(false);
	__input.setReadOnly(false);
	__input.setText("Input");
	__input.addKeyListener(this);

	__output.setMultiLine(true);
	__output.setReadOnly(true);
	__output.setText("Output");

	__lastCounter = 0;

	addAndMakeVisible(__input);
	addAndMakeVisible(__output);
}




ConsoleComponent::~ConsoleComponent()
{
}

void ConsoleComponent::paint(Graphics & ) {
}

void ConsoleComponent::resized()
{
	__input.setBounds(getLocalBounds().removeFromBottom(25));
	__output.setBounds(getLocalBounds().removeFromTop(getLocalBounds().getHeight() - 25));
}

bool ConsoleComponent::keyPressed(const KeyPress & key, Component * /*originatingComponent*/)
{
	if (key == KeyPress::upKey) {
		if (__lastCommands.size() == 0)
			return true;
		__lastCounter++;
		if ((__lastCommands.size()) <= __lastCounter )
			__lastCounter--;
		__input.setText(__lastCommands[__lastCommands.size() - __lastCounter-1]);
		return true;
	}
	if (key == KeyPress::downKey) {
		if (__lastCommands.size() == 0)
			return true;
		__lastCounter--;
		if (__lastCounter < 0)
			__lastCounter++;
		__input.setText(__lastCommands[__lastCommands.size() - __lastCounter-1]);
		return true;
	}
	if (key == KeyPress::returnKey) {
		String s = __input.getText();

		__lastCommands.push_back(s.toStdString());
		
		if (s == "clear")
		{
			__output.setText("");
		}
		if (s == "help")
		{
			__buffer << "list parameters" << "\n" << "set %parameterID% %value%" << "\n";
			__output.setText(__buffer.str());
		}
		if (s == "list parameters") {
			auto floats = Global->paramHandler->GetAll<AudioParameterFloat>();
			for (std::map<String, AudioParameterFloat* >::iterator it = (*floats).begin(); it != (*floats).end(); ++it) {
				__buffer << it->first << " = " << *(it->second) << "\n";
			}
			auto ints = Global->paramHandler->GetAll<AudioParameterInt>();
			for (std::map<String, AudioParameterInt* >::iterator it = (*ints).begin(); it != (*ints).end(); ++it) {
				__buffer << it->first << " = " << *(it->second) << "\n";
			}
			auto bools= Global->paramHandler->GetAll<AudioParameterBool>();
			for (std::map<String, AudioParameterBool* >::iterator it = (*bools).begin(); it != (*bools).end(); ++it) {
				__buffer << it->first << " = " << *(it->second) << "\n";
			}
			auto choices = Global->paramHandler->GetAll<AudioParameterChoice>();
			for (std::map<String, AudioParameterChoice* >::iterator it = (*choices).begin(); it != (*choices).end(); ++it) {
				__buffer << it->first << " = " << *(it->second) << "\n";
			}
			__output.setText(__buffer.str());
		}
		std::vector<std::string> words = split(s.toStdString(), ' ');
		if (words[0] == "set") {
			setParam(words[1], words[2]);
		}
		__lastCounter = 0;
		__input.setText("");

		return true;
	}

	return false;
}

void setParam(String id, bool /*value*/) {

}
void ConsoleComponent::setParam(String id, std::string value) {
	if (Global->paramHandler->GetAll<AudioParameterFloat>()->operator[](id) != nullptr) {
		AudioParameterFloat* fp = Global->paramHandler->GetAll<AudioParameterFloat>()->operator[](id);
		if (fp == nullptr)
			return;
		(*fp) = stof(value);
		__buffer << id << " = " << (*fp) << "\n";
		__output.setText(__buffer.str());
		
	}
	else if (Global->paramHandler->GetAll<AudioParameterInt>()->operator[](id) != nullptr) {
		Global->paramHandler->GetAll<AudioParameterFloat>()->erase(id);
		AudioParameterInt* fp= Global->paramHandler->GetAll<AudioParameterInt>()->operator[](id);
		if (fp == nullptr)
			return;
		(*fp) = stoi(value);
		__buffer << id << " = " << (*fp) << "\n";
		__output.setText(__buffer.str());
	}
	else if (Global->paramHandler->GetAll<AudioParameterBool>()->operator[](id) != nullptr) {
		Global->paramHandler->GetAll<AudioParameterFloat>()->erase(id);
		Global->paramHandler->GetAll<AudioParameterInt>()->erase(id);
		AudioParameterBool* fp = Global->paramHandler->GetAll<AudioParameterBool>()->operator[](id);
		if (fp == nullptr)
			return;
		(*fp) = stoi(value);
		__buffer << id << " = " << (*fp) << "\n";
		__output.setText(__buffer.str());
	}
	
}

std::vector<std::string> ConsoleComponent::split(const std::string& s, char seperator)
{
	std::vector<std::string> output;

	std::string::size_type prev_pos = 0, pos = 0;

	while ((pos = s.find(seperator, pos)) != std::string::npos)
	{
		std::string substring(s.substr(prev_pos, pos - prev_pos));

		output.push_back(substring);

		prev_pos = ++pos;
	}

	output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word

	return output;
}