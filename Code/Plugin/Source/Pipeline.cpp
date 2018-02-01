#include "Pipeline.h"
#include <math.h>


Pipeline::Pipeline(double rate):
__rate(rate),
__pressed(false),
prev_angle(0)
{
}

void Pipeline::noteCommand(int note, uint8 vel, bool isOn) 
{
	__note = note;
	__vel = vel;
	__pressed = isOn;
	//Magic https://en.wikipedia.org/wiki/MIDI_tuning_standard
	__freq = MidiMessage::getMidiNoteInHertz(note);
	//__freq = pow(2.0, (note - 69) / 12)*440.0;

}


Pipeline::~Pipeline()
{

}


bool Pipeline::isActive() {
	return __active;
}
template<typename T>
juce::AudioBuffer<T> Pipeline::render_block(int samples) {
	AudioBuffer<T> jbuff(2, samples);
	if (__pressed) {
		T delta = __freq * 2 * 3.14 / __rate;
		T* buff[] = {
				new T[samples],
				new T[samples] };


		for (int i = 0; i < samples; i++) {
			prev_angle = (delta + prev_angle);
			//buff[0][i] = buff[1][i] = std::sin(prev_angle)*10;
			jbuff.setSample(0, i, std::sin(prev_angle) * __vel*0.0075);
			jbuff.setSample(1, i, std::sin(prev_angle) * __vel*0.0075);
		}
		
		//jbuff.addFrom(0, 0, buff[0], samples);
		//jbuff.addFrom(1, 0, buff[1], samples);
		delete[] buff[0];
		delete[] buff[1];
	}
	__active = jbuff.getMagnitude(0, samples) < 0.2;
	return jbuff;
}

template  juce::AudioBuffer<double> Pipeline::render_block(int samples);
template  juce::AudioBuffer<float> Pipeline::render_block(int samples);
