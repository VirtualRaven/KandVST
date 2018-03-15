#include "Pipeline.h"
#include <math.h>

template<typename T>
Pipeline<T>::Pipeline(double rate,int maxBuffHint) :
	__rate(rate),
	__delay(0,rate),
	__active(false),
	__maxBuffHint(maxBuffHint)
	
{
	for (int i = 0; i < this->__num_osc; i++) {
		__oscs[i] = std::make_tuple(
				new WavetableOsc(i, rate),
				Global->paramHandler->Get<AudioParameterFloat>(i, "OSC_MIX_AMP"),
				Global->paramHandler->Get<AudioParameterBool>(i, "OSC_MIX_EN")
			);
		__effects[i*__num_effects] = new FilterLP<T>(i, rate, "FILTER_LP");
		__effects[i*__num_effects + 1] = new FilterHP<T>(i, rate, "FILTER_HP");

	}

	tmpBuff.setSize(2, maxBuffHint);

}


template<typename T>
int Pipeline<T>::getNoteNumber()
{
	return __note;
}

template<typename T>
void Pipeline<T>::midiCommand(MidiMessage msg, int offset)
{
	if (msg.isNoteOn())
	{
		__note = msg.getNoteNumber();
		__active = true;
	}

	for (auto obj : __oscs) 
	{
		if (*std::get<2>(obj)) 
		{
			std::get<0>(obj)->AddCommand(msg, offset);
			
		}
	}
}

template<typename T>
Pipeline<T>::Pipeline(Pipeline<T>&& ref) :
__rate(ref.__rate),
__delay(0, ref.__rate),
__active(ref.__active),
__maxBuffHint(ref.__maxBuffHint){
	for (size_t i = 0; i < this->__num_osc; i++) {
		__oscs[i] = ref.__oscs[i];
		for (size_t j = 0; j < this->__num_effects; j++) {
			__effects[i*__num_effects + j] = ref.__effects[i*__num_effects + j];
			ref.__effects[i*__num_effects + j] = nullptr;
		}
		ref.__oscs[i] = std::make_tuple(nullptr, nullptr, nullptr);
	}

}

template<typename T>
Pipeline<T>::~Pipeline()
{
	for (auto&& obj : __oscs) {
		auto && tmp = std::get<0>(obj);
		delete tmp;
		tmp = nullptr;
	}

	for (auto&& obj : __effects) {
		delete obj;
	}

}

template<typename T>
bool Pipeline<T>::isActive() {
	return __active;
}

template<typename T>
void Pipeline<T>::render_block(AudioBuffer<T>& buffer) {
	
	auto len = buffer.getNumSamples();
	
	if (len > tmpBuff.getNumSamples()) {
		tmpBuff.setSize(2, len, false, false, true);
	}

	this->tmpBuff.clear(0, len);
	for (int i = 0; i < __num_osc; i++) {
		auto obj = __oscs[i];
		//Is the osc active?
		if (*std::get<2>(obj)) {

			//Did the osc produce anything?
			if (std::get<0>(obj)->RenderBlock(tmpBuff,len))
			{	
				//Apply the effects
				for (int j = 0; j < __num_effects; j++) {
					__effects[i*__num_effects + j]->RenderBlock(tmpBuff, len, false);
				}
				buffer.addFrom(0, 0, tmpBuff, 0, 0, len, *std::get<1>(obj));
				buffer.addFrom(1, 0, tmpBuff, 1, 0, len, *std::get<1>(obj));
				this->tmpBuff.clear(0, len);
			}
		}
	}
	__delay.RenderBlock(buffer, len, false);

	if (buffer.getMagnitude(0, len) < 0.0001)
		__active = false;
	else
		__active = true;
}


template<typename T>
void Pipeline<T>::RegisterParameters(int ID)
{
	for (size_t i = 0; i < Pipeline::__num_osc; i++)
	{
		Global->paramHandler->RegisterBool(Pipeline::__num_osc*ID + i, "OSC_MIX_EN", "Enable Oscillator", false);
		Global->paramHandler->RegisterFloat(Pipeline::__num_osc * ID + i, "OSC_MIX_AMP", "Oscillator Amplitude", 0.0f, 1.0f, 0.5f);
	}
}

template class Pipeline<double>;
template class Pipeline<float>;