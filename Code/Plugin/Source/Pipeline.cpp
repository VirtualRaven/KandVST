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

	if (isOn) //Snabb fix f�r att l�sa vel == 0 n�r NoteOff
		__vel = vel;
	__pressed = isOn;
	//Magic https://en.wikipedia.org/wiki/MIDI_tuning_standard
	


	/*
		Kn�ppande ljud kan bero p� pl�tsliga steg i ljud signalen, t.ex om en sinus b�rjar med vinkel pi/2 s� sker
		ett initial steg, detta steg ineh�ller en stor m�ngd frekvenser spridda i spektrumet, vilket uppfattas som et kort kn�ppande ljud.
		d�rf�r prev_angle=0 vid en ny ton, detta garantera att tonen b�rjar i origo. Liknande s� hindrar konturgeneratorn att liknande steg sker
		vid note off genom att tona ut signalen l�ngsamt.

		F�r att sammanfatta en enkel l�xa, undvik alla diskontinuitet, de orsakar obehagliga ljud . 
	
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
