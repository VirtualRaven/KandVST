#include "IGenerator.h"


void IGenerator::AddNoteCommand(int offset, int note, uint8 vel, bool isOn) {
	this->__messages.push(noteMessage(offset, note, vel, isOn));
}

int IGenerator::HandleEvent() {
	if (!this->__messages.empty()) {
		auto tmp = this->__messages.front();
		this->__messages.pop();
		this->ProccesNoteCommand(tmp.note, tmp.vel, tmp.isOn);
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