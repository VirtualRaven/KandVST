/*
 * This file is part of the KandVST synthesizer.
 *
 * Copyright (c) 2018   Lukas Rahmn, Anton Fredriksson,
 *                      Sarosh Nasir, Stanisław Zwierzchowski,
 *                      Klas Ludvigsson and Andreas Kuszli
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "WavetableOsc.h"
#include "Wavetable.h"
#include "LFO.h" 

WavetableOsc::WavetableOsc(int ID, double sampleRate,int maxBuffHint,GLOBAL*global) :
	IGenerator(sampleRate),
	IVSTParameters(ID),
	__maxBuffHint(maxBuffHint),
	__envelope(__ID, sampleRate,global),
	__note(0),
	__sustainPedal(false),
	__keyDown(false),
	__phase(0),
	__frequency(0),
	__noiseBuffer(static_cast<int>(sampleRate * 2)),
	__rand(),
	__rand_index(0),
	__pitchbend(0),
	ourLookAndFeel(global)
{
	Global = global;
	__waveType = Global->paramHandler->Get<AudioParameterInt>(__ID, "WAVE_TYPE");
	__octave = Global->paramHandler->Get<AudioParameterInt>(__ID, "OSC_OCTAVE");
	__offset = Global->paramHandler->Get<AudioParameterInt>(__ID, "OSC_OFFSET");
	__detune = Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_DETUNE");
	__overtone = Global->paramHandler->Get<AudioParameterInt>(__ID, "OSC_OVERTONE");
	__panning = Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_PAN");
	__pitchBendSens = Global->paramHandler->Get<AudioParameterInt>(-1, "PITCH_BEND_SENS");

	__sinAmp = Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_SINE");
	__sqAmp = Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_SQUARE");
	__sawAmp = Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_SAW");
	__triAmp = Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_TRI");
	__noiseAmp = Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_NOISE");

	__lfofreq = Global->paramHandler->Get <AudioParameterChoice>(__ID, "OSC_LFO_FREQ");
	__lfoamp = Global->paramHandler->Get <AudioParameterChoice>(__ID, "OSC_LFO_AMP");

	__envBuff = new double[maxBuffHint];

	// Don't Generate wavetable
	//for (auto&& samp : __noiseBuffer)
		//samp = (__rand.nextDouble() - 0.5f) * 2.0f;
	for (size_t i = 0; i < __noiseData.size(); i++)
	{
		__noiseData[i] = (__rand.nextDouble() - 0.5f) * 2.0f;

	}
}


WavetableOsc::WavetableOsc(WavetableOsc && ref) :
	IGenerator(ref.__sampleRate),
	IVSTParameters(ref.__ID),
	__maxBuffHint(ref.__maxBuffHint),
	__envelope(__ID, ref.__sampleRate,ref.Global),
	__note(ref.__note),
	__sustainPedal(ref.__sustainPedal),
	__keyDown(ref.__keyDown),
	__phase(ref.__phase),
	__frequency(ref.__frequency),
	__noiseBuffer(static_cast<int>(ref.__sampleRate * 2)),
	__rand(),
	__rand_index(ref.__rand_index),
	__pitchbend(ref.__pitchbend),
	ourLookAndFeel(ref.Global)
{
	__waveType = Global->paramHandler->Get<AudioParameterInt>(__ID, "WAVE_TYPE");
	__octave = Global->paramHandler->Get<AudioParameterInt>(__ID, "OSC_OCTAVE");
	__offset = Global->paramHandler->Get<AudioParameterInt>(__ID, "OSC_OFFSET");
	__detune = Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_DETUNE");
	__overtone = Global->paramHandler->Get<AudioParameterInt>(__ID, "OSC_OVERTONE");
	__panning = Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_PAN");
	__pitchBendSens = Global->paramHandler->Get<AudioParameterInt>(-1, "PITCH_BEND_SENS");

	__sinAmp = Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_SINE");
	__sqAmp = Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_SQUARE");
	__sawAmp = Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_SAW");
	__triAmp = Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_TRI");
	__noiseAmp = Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_NOISE");

	__lfofreq = Global->paramHandler->Get <AudioParameterChoice>(__ID, "OSC_LFO_FREQ");
	__lfoamp = Global->paramHandler->Get <AudioParameterChoice>(__ID, "OSC_LFO_AMP");

	__envBuff = ref.__envBuff;
	ref.__envBuff = nullptr;



	
}

WavetableOsc::~WavetableOsc()
{
	delete[] __envBuff;
}


void WavetableOsc::renderImage(Image* image)
{
	int width = image->getWidth();
	int height = image->getHeight();
	double max = 0.0;
	double* data = new double[width];


	for (int i = 0; i < width; i++)
	{
		int ind = (i * tables[0]->getLength() / width);

		double samp = tables[WAVE_TYPE::SINE]->__tables[0][ind] * (*__sinAmp);
		samp += tables[WAVE_TYPE::SQUARE]->__tables[0][ind] * (*__sqAmp);
		samp += tables[WAVE_TYPE::SAW]->__tables[0][ind] * (*__sawAmp);
		samp += tables[WAVE_TYPE::TRI]->__tables[0][ind] * (*__triAmp);
		samp += __noiseData[ind] * *__noiseAmp;
		data[i] = samp;

		max = jmax<double>(max, samp);
		max = jmax<double>(max, samp*-1);
	}

	Graphics g(*image);
	g.setColour(Colour::fromRGB(36, 36, 36));
	g.fillAll();

	Path p;

	g.setColour(Colour::fromRGBA(255, 255, 255, 15));
	for (size_t i = 1; i <= 10; i++)
	{
		
		g.drawLine((image->getWidth() / 10)*i, 0, (image->getWidth() / 10)*i, image->getHeight());
	}
	for (size_t i = 1; i <= 10; i++)
	{
		g.drawLine(0, (image->getHeight() / 10)*i, image->getWidth(), (image->getHeight() / 10)*i);
	}

	int colourIndex = Global->paramHandler->Get<AudioParameterChoice>(-1, "THEME")->getIndex();
	g.setColour(__themePicker.getColour(colourIndex));
	g.setImageResamplingQuality(Graphics::highResamplingQuality);

	double hMul = (static_cast<double>(height - 32)/2.0) / (max);
	if (max < 0.0001 || max < 1.00)
		hMul = (height - 32) / 2.0;

	double lastx = 0, lasty=(height - 32)/2;
	for (int i = 0; i < width; i++)
	{
		if (i > 0)
			p.lineTo(static_cast<float>(i), static_cast<float>(height / 2 - hMul * data[i]));
			/*g.drawLine(
				static_cast<float>(lastx),
				static_cast<float>(lasty),
				static_cast<float>(i),
				static_cast<float>(height / 2 - hMul*data[i]),
				6);*/
		else
		{
			p.startNewSubPath(i, height / 2 - hMul * data[i]);
		}
		lastx = i;
		lasty = height / 2 - hMul*data[i];
	}

	PathStrokeType pst(6.0f);
	pst.setJointStyle(PathStrokeType::JointStyle::curved);
	g.strokePath(p, pst);
	p.lineTo(image->getWidth(), image->getHeight());
	p.lineTo(0, image->getHeight());

	p.closeSubPath();
	//g.setFillType(FillType(ColourGradient(Colour::fromRGBA(26, 105, 180, 80), 0, 0, Colour::fromRGBA(26, 105, 180, 10), 0, image->getHeight(), false)));
	//g.fillPath(p);

	g.setColour(Colour::fromRGB(26, 26, 26));
	g.drawRect(0, 0, width, height, 6);
	delete[] data;

}
void WavetableOsc::Reset()
{
	__envelope.Stop();
	this->clearMidi();
}


void WavetableOsc::ProccessCommand(MidiMessage msg)
{
	if (msg.isNoteOn())
	{
		__note = msg.getNoteNumber();
		__frequency = MidiMessage::getMidiNoteInHertz(__note);
		__envelope.Reset(msg.getVelocity());
		__keyDown = true;
	}
	else if (msg.isNoteOff())
	{
		if (msg.getNoteNumber() == __note)
			__keyDown = false;
	}
	else if (msg.isSustainPedalOn())
	{
		__sustainPedal = true;
	}
	else if (msg.isSustainPedalOff())
	{
		__sustainPedal = false;
	}
	else if (msg.isPitchWheel())
	{
		//Convert from 14 bit unsigned int to float between -1.0 and 1.0
		__pitchbend = ((float)msg.getPitchWheelValue() / 16383)*2.0f - 1.0f;
	}

	__envelope.setSustain(__sustainPedal || __keyDown);
}

void WavetableOsc::RegisterParameters(int ID, GLOBAL*Global)
{
	Global->paramHandler->RegisterInt(ID, "WAVE_TYPE", "Wave type", 0, 3, 0);
	Global->paramHandler->RegisterInt(ID, "OSC_OCTAVE", "Octave", -3, 3, 0);
	Global->paramHandler->RegisterInt(ID, "OSC_OFFSET", "Offset", -11, 11, 0);
	Global->paramHandler->RegisterFloat(ID, "OSC_DETUNE", "Detune", -1.0f, 1.0f, 0.0f);
	Global->paramHandler->RegisterInt(ID, "OSC_OVERTONE", "Overtone", 0, 6, 0);
	Global->paramHandler->RegisterFloat(ID, "OSC_PAN", "Panning", -1.0f, 1.0f, 0.0f);

	Global->paramHandler->RegisterFloat(ID, "OSC_SINE", "Sine", 0.0f, 1.0f, 1.0f);
	Global->paramHandler->RegisterFloat(ID, "OSC_SQUARE", "Square", 0.0f, 1.0f, 0.0f);
	Global->paramHandler->RegisterFloat(ID, "OSC_SAW", "Saw", 0.0f, 1.0f, 0.0f);
	Global->paramHandler->RegisterFloat(ID, "OSC_TRI", "Tri", 0.0f, 1.0f, 0.0f);
	Global->paramHandler->RegisterFloat(ID, "OSC_NOISE", "Noise", 0.0f, 1.0f, 0.0f);
	
	StringArray choices("OFF", "LFO_1", "LFO_2");
	Global->paramHandler->RegisterChoice(ID, "OSC_LFO_FREQ", "Lfo Frequency", choices, 0);
	Global->paramHandler->RegisterChoice(ID, "OSC_LFO_AMP", "Lflo Amplitude", choices, 0);
	

}


template<typename T>
bool WavetableOsc::__RenderBlock(AudioBuffer<T>& buffer,int len) {
	
	
	auto buffs = buffer.getArrayOfWritePointers();

	auto nextEvent = this->getNextEventOffset();
	auto numSampels = len;

	float gains[5] = { *__sinAmp , *__sqAmp,*__sawAmp,*__triAmp, *__noiseAmp };
	double calcFreq = __frequency * pow(2.0, *__octave + (((*__offset) + (*__detune) + (*__pitchBendSens * __pitchbend)) / 12.0)) * ((*__overtone)+1);
	double tmpInc = IWavetable::getLength() / __sampleRate;


	bool dataGenerated = false;
	
	float tmpAmp = gains[0] + gains[1] + gains[2] + gains[3] + gains[4];
	if (tmpAmp > 1)
	{
		for (size_t i = 0; i < 4; i++)
		{
			if (gains[i] != 0)
				gains[i] /= tmpAmp;
		}
	}

	float leftSpeaker = std::min(1.0f, 1.0f - (*__panning));
	float rightSpeaker = std::min(1.0f, 1.0f + (*__panning));

	double* lfofreq, *lfoamp;

	int lfoFreqIndx = (*__lfofreq).getIndex();
	float lfoFreqAmount = 0.0f;
	if (lfoFreqIndx != 0) {
		lfofreq = Global->lfos[lfoFreqIndx - 1]->getPointer();
		lfoFreqAmount = Global->lfos[lfoFreqIndx-1]->getAmount();
	}
	int lfoAmpIndx = (*__lfoamp).getIndex();
	float lfoAmpAmount = 0.0f;
	if (lfoAmpIndx != 0) {
		lfoamp = Global->lfos[lfoAmpIndx - 1]->getPointer();
		lfoAmpAmount = Global->lfos[lfoAmpIndx-1]->getAmount();
	}
	
	int envValidUntil = -1;
	int envOffset = 0;

	for (int i = 0; i < numSampels; i++)
	{
		if (i == nextEvent) {
			nextEvent = this->HandleEvent();
			calcFreq = __frequency * pow(2.0, *__octave + (((*__offset) + (*__detune) + (*__pitchBendSens * __pitchbend)) / 12.0)) * ((*__overtone) + 1);
		}

		//This code makes sure that we do not render anything
		//If the envelope isn't active we skip forward to the next midi event
		//or if no other midi event exists we exit the function 
		if (!__envelope.isActive()) {
			if (nextEvent < numSampels && nextEvent  > i) {
				i = nextEvent;
				nextEvent = this->HandleEvent();
				calcFreq = __frequency * pow(2.0, *__octave + (((*__offset) + (*__detune) + (*__pitchBendSens * __pitchbend)) / 12.0)) * ((*__overtone) + 1);
			}
			else break; 
		}
		
		//Have we rendered envelope info for this part of the buffer?
		if (envValidUntil <= i) {
			envOffset = -i;
			size_t envLen = nextEvent == -1 ? len - i : nextEvent - i;
			__envelope.RenderBlock(this->__envBuff, envLen);
			envValidUntil = i + envLen;
		}

		double tmpFreq = calcFreq;

		//Calculate lfo frequency effect if it's active
		if (lfoFreqIndx != 0 && lfofreq)
		{
			tmpFreq *= pow(2, lfofreq[i]*lfoFreqAmount);
		}
		
		double inc = tmpInc * tmpFreq;

		auto tgt = IWavetable::getLoc(__phase, tmpFreq);

		
		
		
		
		double tmp_samp = getSampleFromLoc<SINE>(tgt) *gains[0];
		tmp_samp += getSampleFromLoc<SQUARE>(tgt) *gains[1];
		tmp_samp += getSampleFromLoc<SAW>(tgt) *gains[2];
		tmp_samp += getSampleFromLoc<TRI>(tgt) *gains[3];
		// No wavetable for noise
		//tmp_samp += __noiseBuffer[__rand_index++] * gains[4];
		tmp_samp += (__rand.nextDouble() - 0.5f) * 2.0f * gains[4];

		//Calculate lfo amplitude effect if it's active
		if (lfoAmpAmount > 0.0f) {
			//tmp_samp += tmp_samp*lfoamp[i]*lfoAmpAmount;
			tmp_samp = tmp_samp - tmp_samp*lfoAmpAmount*((lfoamp[i] + 1.0) / 2.0);
		}

		tmp_samp *= __envBuff[envOffset+i];

		T samp = static_cast<T>(tmp_samp);
		__phase += inc;
		__rand_index = __rand_index % __noiseBuffer.size();

		if (buffer.getNumChannels() == 2) {
			buffs[0][i] = samp * leftSpeaker;
			buffs[1][i] = samp * rightSpeaker;
		}
		else{
			for (int j = 0; j < buffer.getNumChannels(); j++)
				buffs[j][i]= samp;
		}
		dataGenerated = true;
		
	}

	
	return dataGenerated;
}

template bool WavetableOsc::__RenderBlock(AudioBuffer<double>& buffer,int len);
template bool WavetableOsc::__RenderBlock(AudioBuffer<float>& buffer, int len);

