/*
 * This file is part of the KandVST synthesizer.
 *
 * Copyright (c) 2018   Lukas Rahmn, Anton Fredriksson,
 *                      Sarosh Nasir, Stanisław Zwierzchowski,
 *                      Klas Ludvigsson and Andreas Kuszli
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "IGenerator.h"


void IGenerator::AddCommand(MidiMessage msg, int offset) {
	this->__messages.push(midiCommand(msg, offset));
}

int IGenerator::HandleEvent() {
	if (!this->__messages.empty()) {
		auto offset = this->__messages.front().offset;
		while (!this->__messages.empty() && this->__messages.front().offset == offset) {
			auto tmp = this->__messages.front();
			this->__messages.pop();
			this->ProccessCommand(tmp.msg);
		}
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

void IGenerator::clearMidi() {
	while (!this->__messages.empty())
		this->__messages.pop();
}