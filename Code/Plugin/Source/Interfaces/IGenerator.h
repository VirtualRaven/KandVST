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

#ifndef IGENERATOR_H
#define IGENERATOR_H
#include "../JuceLibraryCode/JuceHeader.h"
#include <queue>
class IGenerator
{
private:
	/*struct noteMessage {
		int offset;
		int note;
		uint8 vel;
		bool isOn;
		noteMessage(int offset, int note, uint8 vel, bool isOn) :
			offset(offset),
			note(note),
			vel(vel),
			isOn(isOn) {}
		noteMessage() :
			offset(0),
			note(0),
			vel(0),
			isOn(true) {}
	};*/

	struct midiCommand {
		MidiMessage msg;
		int offset;
		midiCommand(MidiMessage msg, int offset) :
			msg(msg),
			offset(offset) {}
	};

	IGenerator();
	std::queue<midiCommand> __messages;
protected:
	double __sampleRate;
public:
	virtual ~IGenerator() {};
	IGenerator(double sampleRate) : __messages(), 
									__sampleRate(sampleRate) {};
	virtual bool RenderBlock(AudioBuffer<float>& buffer, int len) = 0;
	virtual bool RenderBlock(AudioBuffer<double>& buffer,int len) = 0;

	virtual void AddCommand(MidiMessage msg, int offset);
	virtual const char * name() const =0;
	int HandleEvent(); //Processes the current event and returns the index of the next event to be handled
	int getNextEventOffset(); //Get the buffer offset of the next event to occur
	virtual void ProccessCommand(MidiMessage msg) = 0;
	void clearMidi();
	virtual void Reset()=0;

};

#endif //IGENERATOR_H

