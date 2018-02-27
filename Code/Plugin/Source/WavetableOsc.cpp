#include "WavetableOsc.h"
#include "Wavetable.h"
 
WavetableOsc::WavetableOsc(int ID, double sampleRate) :
	IGenerator(sampleRate),
	IVSTParameters(ID),
	__envelope(__ID,sampleRate),
	__note(0),
	__sustain(false),
	__wavetable(tables[WAVE_TYPE::SAW]),
	__phase(0),
	__frequency(0),
	__lfo(120,sampleRate, __ID),
	__noiseBuffer(static_cast<int>(sampleRate)),
	__rand(174594158),
	__rand_index(0)

{
	__waveType = Global->paramHandler->Get<AudioParameterInt>(__ID, "WAVE_TYPE");
	__octave = Global->paramHandler->Get<AudioParameterInt>(__ID, "OSC_OCTAVE");
	__offset = Global->paramHandler->Get<AudioParameterInt>(__ID, "OSC_OFFSET");
	__detune = Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_DETUNE");

	__sinAmp = Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_SINE");
	__sqAmp = Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_SQUARE");
	__sawAmp = Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_SAW");
	__triAmp = Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_TRI");
	__noiseAmp = Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_NOISE");

	// Generate wavetable
	for (auto&& samp : __noiseBuffer)
		samp = (__rand.nextDouble() - 0.5f) * 2.0f;

}


WavetableOsc::~WavetableOsc()
{
}

void WavetableOsc::setWaveform(WAVE_TYPE t)
{
	if (t == WAVE_TYPE::__COUNT) return;
	__wavetable = tables[t];
}
void WavetableOsc::renderImage(Image* image,int width, int height)
{

	image->clear(Rectangle<int>(width, height));


	double max = 0.0;
	double* data = new double[width];

	for (int i = 0; i < width; i++)
	{
		int ind = (i * 2048 / width);

		double samp = tables[WAVE_TYPE::SINE]->__tables[0][ind] * (*__sinAmp);
		samp += tables[WAVE_TYPE::SQUARE]->__tables[0][ind] * (*__sqAmp);
		samp += tables[WAVE_TYPE::SAW]->__tables[0][ind] * (*__sawAmp);
		samp += tables[WAVE_TYPE::TRI]->__tables[0][ind] * (*__triAmp);

		data[i] = samp;

		max = jmax<double>(max, samp);
		max = jmax<double>(max, samp*-1);
	}

	Graphics g(*image);
	g.setColour(Colour::fromRGB(36, 36, 36));
	g.fillAll();
	g.setColour(Colour::fromRGB(26,105,180));
	g.setImageResamplingQuality(Graphics::highResamplingQuality);

	double hMul = (static_cast<double>(height - 16)/2.0) / (max);
	if (max < 0.0001 || max < 1.00)
		hMul = (height - 16) / 2.0;

	double lastx = 0, lasty=(height - 16)/2;
	for (int i = 0; i < width; i++)
	{
		if (i > 0)
			
			g.drawLine(lastx, lasty, i, height / 2 - hMul*data[i], 3);
		lastx = i;
		lasty = height / 2 - hMul*data[i];
	}

	g.setColour(Colour::fromRGB(26, 26, 26));
	g.drawRect(0, 0, width, height, 3);
	delete data;

}
/*void WavetableOsc::setOctave(int o)
{
	if (o > 3 || o < -3) return;
	__octave = o;
}*/

void WavetableOsc::ProccesNoteCommand(int note, uint8 vel, bool isOn)
{
	if (isOn)
	{
		__frequency = MidiMessage::getMidiNoteInHertz(note);
		//__phase = 0.0;
		__note = note;
		__envelope.Reset(vel);
		__sustain = true; //Right now we ignore sustain pedal
	}
	else if (!isOn && note == __note) {
		__sustain = false;
	}
	

}

void WavetableOsc::ProccessCommand(MidiMessage message)
{
}

void WavetableOsc::RegisterParameters(int ID)
{
	Global->paramHandler->RegisterInt(ID, "WAVE_TYPE", "Wave type", 0, 3, 0);
	Global->paramHandler->RegisterInt(ID, "OSC_OCTAVE", "Octave", -3, 3, 0);
	Global->paramHandler->RegisterInt(ID, "OSC_OFFSET", "Offset", -11, 11, 0);
	Global->paramHandler->RegisterFloat(ID, "OSC_DETUNE", "Detune", -1.0f, 1.0f, 0.0f);

	Global->paramHandler->RegisterFloat(ID, "OSC_SINE", "Sine", 0.0f, 1.0f, 1.0f);
	Global->paramHandler->RegisterFloat(ID, "OSC_SQUARE", "Square", 0.0f, 1.0f, 0.0f);
	Global->paramHandler->RegisterFloat(ID, "OSC_SAW", "Saw", 0.0f, 1.0f, 0.0f);
	Global->paramHandler->RegisterFloat(ID, "OSC_TRI", "Tri", 0.0f, 1.0f, 0.0f);
	Global->paramHandler->RegisterFloat(ID, "OSC_NOISE", "Noise", 0.0f, 1.0f, 0.0f);

}


template<typename T>
void WavetableOsc::__RenderBlock(AudioBuffer<T>& buffer,double gain) {
	
	
	setWaveform(toWAVE_TYPE(*__waveType));
	auto buffs = buffer.getArrayOfWritePointers();

	auto nextEvent = this->getNextEventOffset();
	auto numSampels = buffer.getNumSamples();

	float gains[5] = { *__sinAmp , *__sqAmp,*__sawAmp,*__triAmp, *__noiseAmp };
	double calcFreq = __frequency * pow(2.0, *__octave + (((*__offset) + (*__detune)) / 12.0));
	double tmpInc = __wavetable->getLength() / __sampleRate;

	for (int i = 0; i < numSampels; i++)
	{
		if (i == nextEvent) {
			nextEvent = this->HandleEvent();
			calcFreq = __frequency * pow(2.0, *__octave + (((*__offset) + (*__detune)) / 12.0));
			
		}

		//This code makes sure that we do not render anything
		//If the envelope isn't active we skip forward to the next midi event
		//or if no other midi event exists we exit the function 
		if (!__envelope.isActive()) {
			if (nextEvent < numSampels && nextEvent  > i) {
				i = nextEvent;
				nextEvent = this->HandleEvent();
				calcFreq = __frequency * pow(2.0, *__octave + (((*__offset) + (*__detune)) / 12.0));
			}
			else break; 
		}
	

		
		double tmpFreq = calcFreq;
		__lfo.apply(tmpFreq);
		double inc = tmpInc * tmpFreq;

		auto tgt = IWavetable::getLoc(__phase, tmpFreq);


		double tmp_samp = getSampleFromLoc<SINE>(tgt) *gains[0];
		tmp_samp += getSampleFromLoc<SQUARE>(tgt) *gains[1];
		tmp_samp += getSampleFromLoc<SAW>(tgt) *gains[2];
		tmp_samp += getSampleFromLoc<TRI>(tgt) *gains[3];
		tmp_samp += __noiseBuffer[__rand_index++] * gains[4];
		tmp_samp *= __envelope.GenerateNextStep(__sustain) * gain;
		T samp = static_cast<T>(tmp_samp);
		__phase += inc;
		__rand_index = __rand_index % __noiseBuffer.size();

		if (buffer.getNumChannels() == 2) {
			buffs[0][i] += samp;
			buffs[1][i] += samp;
		}
		else{
			for (int j = 0; j < buffer.getNumChannels(); j++)
				buffs[j][i]+= samp;
		}
		
	}
}

template void WavetableOsc::__RenderBlock(AudioBuffer<double>& buffer,double gain);
template void WavetableOsc::__RenderBlock(AudioBuffer<float>& buffer,double gain);

