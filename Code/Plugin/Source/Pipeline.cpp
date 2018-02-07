#include "Pipeline.h"
#include <math.h>


Pipeline::Pipeline(double rate) :
	__rate(rate),
	__osc(rate),
	__delay(rate)
{
}

void Pipeline::noteCommand(int note, uint8 vel, bool isOn) 
{
	__osc.ProccesNoteCommand(note, vel, isOn);
}


Pipeline::~Pipeline()
{

}


bool Pipeline::isActive() {
	return true;
}

template<typename T>
void Pipeline::render_block(AudioBuffer<T>& buffer) {
	__osc.RenderBlock(buffer);
	__delay.RenderBlock(buffer);

}

template void Pipeline::render_block(AudioBuffer<double>& buffer);
template void Pipeline::render_block(AudioBuffer<float>& buffer);
