#ifndef IGENERATOR_H
#define IGENERATOR_H
#include "../JuceLibraryCode/JuceHeader.h"
#include <queue>
class IGenerator
{
private:
	struct noteMessage {
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
	};
	IGenerator();
	std::queue<noteMessage> __messages;
protected:
	double __sampleRate;
public:
	virtual ~IGenerator() {};
	IGenerator(double sampleRate) : __messages(), 
									__sampleRate(sampleRate) {};
	virtual bool RenderBlock(AudioBuffer<float>& buffer, int len) = 0;
	virtual bool RenderBlock(AudioBuffer<double>& buffer,int len) = 0;

	virtual void ProccesNoteCommand( int note, uint8 vel, bool isOn) = 0;
	virtual void AddNoteCommand(int offset, int note, uint8 vel, bool isOn);
	virtual const char * name() const =0;
	int HandleEvent(); //Processes the current event and returns the index of the next event to be handled
	int getNextEventOffset(); //Get the buffer offset of the next event to occur
	virtual void ProccessCommand(MidiMessage message) = 0;
};

#endif //IGENERATOR_H

