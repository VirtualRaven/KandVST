#include "Pipeline.h"
#include <math.h>


Pipeline::Pipeline(double rate) :
	__rate(rate),
	__delay(0,rate),
	__active(false)
{
	for (int i = 0; i < this->__num_osc; i++) {
		__oscs[i] = std::make_tuple(
				new WavetableOsc(i, rate),
				Global->paramHandler->Get<AudioParameterFloat>(i, "OSC_MIX_AMP"),
				Global->paramHandler->Get<AudioParameterBool>(i, "OSC_MIX_EN")
			);
	}
}

int Pipeline::getNoteNumber()
{
	return __note;
}

void Pipeline::noteCommand(int offset, int note, uint8 vel, bool isOn) 
{
	if (isOn) {
		__note = note;
		__active = true;
	}
		
	for (auto obj : __oscs) {
		if (*std::get<2>(obj)) {
			std::get<0>(obj)->AddNoteCommand(offset, note, vel, isOn);
		}
	}
}

Pipeline::Pipeline(Pipeline&& ref) :
__rate(ref.__rate),
__delay(0, ref.__rate),
__active(ref.__active) {
	for (size_t i = 0; i < this->__num_osc; i++) {
		__oscs[i] = ref.__oscs[i];
		ref.__oscs[i] = std::make_tuple(nullptr, nullptr, nullptr);
	}

}


Pipeline::~Pipeline()
{
	for (auto&& obj : __oscs) {
		auto && tmp = std::get<0>(obj);
		delete tmp;
		tmp = nullptr;
	}

}


bool Pipeline::isActive() {
	return __active;
}

template<typename T>
void Pipeline::render_block(AudioBuffer<T>& buffer) {
	for (auto obj : __oscs) {
		if (*std::get<2>(obj)) {
			std::get<0>(obj)->RenderBlock(buffer,*std::get<1>(obj));
		}
	}
	__delay.RenderBlock(buffer);

	if (buffer.getMagnitude(0, buffer.getNumSamples()) < 0.0001)
		__active = false;
	else
		__active = true;
}

template void Pipeline::render_block(AudioBuffer<double>& buffer);
template void Pipeline::render_block(AudioBuffer<float>& buffer);

void Pipeline::RegisterParameters(int ID)
{
	for (size_t i = 0; i < Pipeline::__num_osc; i++)
	{
		Global->paramHandler->RegisterBool(Pipeline::__num_osc*ID + i, "OSC_MIX_EN", "Enable Oscillator", false);
		Global->paramHandler->RegisterFloat(Pipeline::__num_osc * ID + i, "OSC_MIX_AMP", "Oscillator Amplitude", 0.0f, 1.0f, 0.5f);
	}
}
