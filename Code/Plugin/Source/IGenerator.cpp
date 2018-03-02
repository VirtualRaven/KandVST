#include "IGenerator.h"


void IGenerator::AddCommand(MidiMessage msg, int offset) {
	this->__messages.push(midiCommand(msg, offset));
}

int IGenerator::HandleEvent() {
	if (!this->__messages.empty()) {
		auto tmp = this->__messages.front();
		this->__messages.pop();
		this->ProccessCommand(tmp.msg);
		if (!this->__messages.empty())
			return this->__messages.front().offset;
		else
			return -1;

	}
	else
		throw std::underflow_error("No events left to be handled");
}

int IGenerator::getNextEventOffset() {
	if (!this->__messages.empty())
		return this->__messages.front().offset;
	else
		return -1;
}