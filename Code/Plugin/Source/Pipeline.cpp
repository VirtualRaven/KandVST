#include "Pipeline.h"
#include <math.h>


Pipeline::Pipeline(double rate) :
	__rate(rate),
	__osc(0,rate),
	__delay(0,rate),
	_active(false)
{
}

void Pipeline::noteCommand(int offset, int note, uint8 vel, bool isOn) 
{
	__osc.AddNoteCommand(offset,note, vel, isOn);
	_active = isOn;
}


Pipeline::~Pipeline()
{

}


bool Pipeline::isActive() {
	return _active;
}

template<typename T>
void Pipeline::render_block(AudioBuffer<T>& buffer) {
	__osc.RenderBlock(buffer);
	__delay.RenderBlock(buffer);
}

template void Pipeline::render_block(AudioBuffer<double>& buffer);
template void Pipeline::render_block(AudioBuffer<float>& buffer);
