#include "Pipeline.h"
#include <math.h>


Pipeline::Pipeline(double rate) :
	__rate(rate),
	__freq(0),
	__pressed(false),
	prev_angle(0),
	amplitude(0),
	__env(rate)
{
}

void Pipeline::noteCommand(int note, uint8 vel, bool isOn) 
{
	__note = note;

	if (isOn) //Snabb fix för att lösa vel == 0 när NoteOff
		__vel = vel;
	__pressed = isOn;
	//Magic https://en.wikipedia.org/wiki/MIDI_tuning_standard
	


	/*
		Knäppande ljud kan bero på plötsliga steg i ljud signalen, t.ex om en sinus börjar med vinkel pi/2 så sker
		ett initial steg, detta steg inehåller en stor mängd frekvenser spridda i spektrumet, vilket uppfattas som et kort knäppande ljud.
		därför prev_angle=0 vid en ny ton, detta garantera att tonen börjar i origo. Liknande så hindrar konturgeneratorn att liknande steg sker
		vid note off genom att tona ut signalen långsamt.

		För att sammanfatta en enkel läxa, undvik alla diskontinuitet, de orsakar obehagliga ljud . 
	
	*/
	if (isOn) {
		__freq = MidiMessage::getMidiNoteInHertz(note);
		//__freq = pow(2.0, (note - 69) / 12)*440.0;
		prev_angle = 0;
		__env.Reset();
	}
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

		
		for (int i = 0; i < samples; i++) {
			prev_angle = fmod((delta + prev_angle),2*3.14);
			double tmp = std::sin(prev_angle)* __vel*0.0075*__env.GenerateNextStep(__pressed);
			jbuff.setSample(0, i, tmp);
			jbuff.setSample(1, i, tmp);
		}
	
	__active = jbuff.getMagnitude(0, samples) < 0.2;
	return jbuff;
}

template  juce::AudioBuffer<double> Pipeline::render_block(int samples);
template  juce::AudioBuffer<float> Pipeline::render_block(int samples);
