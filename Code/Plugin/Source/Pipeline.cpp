#include "Pipeline.h"
#include <math.h>


Pipeline::Pipeline(double rate) :
	__rate(rate),
	__freq(0),
	__pressed(false),
	prev_angle(0),
	amplitude(0)
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
	T delta = __freq * 2 * 3.14 / __rate;
	if (__pressed) {
		amplitude = 1.0;
		
		for (int i = 0; i < samples; i++) {
			prev_angle = fmod((delta + prev_angle),2*3.14);
			jbuff.setSample(0, i, std::sin(prev_angle) * __vel*0.0075*amplitude);
			jbuff.setSample(1, i, std::sin(prev_angle) * __vel*0.0075*amplitude);
		}

	} 
	else
	{
		for (int i = 0; i < samples; i++) {
			
			prev_angle = fmod((delta + prev_angle), 2 * 3.14);
			jbuff.setSample(0, i, std::sin(prev_angle) * 127*0.0075*amplitude);
			jbuff.setSample(1, i, std::sin(prev_angle) * 127*0.0075*amplitude);
			amplitude *= 0.9999;
		}
	}
	__active = jbuff.getMagnitude(0, samples) < 0.2;
	return jbuff;
}

template  juce::AudioBuffer<double> Pipeline::render_block(int samples);
template  juce::AudioBuffer<float> Pipeline::render_block(int samples);
