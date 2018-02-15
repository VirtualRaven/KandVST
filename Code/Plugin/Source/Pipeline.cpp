#include "Pipeline.h"
#include <math.h>


Pipeline::Pipeline(double rate) :
	__rate(rate),
	__osc(0,rate),
	__delay(0,rate),
	_active(false)
{
}

int Pipeline::getNoteNumber()
{
	return __note;
}

void Pipeline::noteCommand(int offset, int note, uint8 vel, bool isOn) 
{
	if (isOn)
		__note = note;

	__osc.AddNoteCommand(offset,note, vel, isOn);
	_active = isOn;
}


Pipeline::~Pipeline()
{

}


bool Pipeline::isActive() {
	return __osc.isActive();
}

template<typename T>
void Pipeline::render_block(AudioBuffer<T>& buffer) {
	__osc.RenderBlock(buffer);
	__delay.RenderBlock(buffer);
}

template void Pipeline::render_block(AudioBuffer<double>& buffer);
template void Pipeline::render_block(AudioBuffer<float>& buffer);
