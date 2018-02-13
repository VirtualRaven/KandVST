#include "Pipeline.h"
#include <math.h>
#include <mutex>


Pipeline::Pipeline(double rate) :
	__rate(rate),
	__osc(rate),
	__delay(rate),
	_active(false)

{
}

void Pipeline::noteCommand(int note, uint8 vel, bool isOn) 
{
	__osc.ProccesNoteCommand(note, vel, isOn);
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
