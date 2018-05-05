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

#include "EnvelopeGenerator.h"
#define S_LEVEL 0.5
#define S_ENDLEVEL 0.0
#define A_LEVEL 1.0



double EnvelopeGenerator::GenerateNextStep(bool sustain)
{
	int __Attack = (int)(*__a_time * __sampleRate);
	int __Decay = (int)(*__d_time * __sampleRate);
	int __sustain = (int)(*__h_time * __sampleRate);
	int __Release = (int)(*__r_time * __sampleRate);

	switch (__state)
	{
	case 0:
		if (!sustain) {
			__state = 3;
			break;
		}

		__amplitude +=calculateSlope(0, A_LEVEL, __Attack);

		if (__amplitude >= A_LEVEL) {
			__state++;
		}
		break;
	case 1:
		if (!sustain) {
			__state = 3;
			break;
		}

		__amplitude +=calculateSlope(A_LEVEL, S_LEVEL, __Decay);

		if (__amplitude <= S_LEVEL) {
			__state++;
		}
		break;
	case 2:
		if (!sustain) {
			__state = 3;
			break;
		}
		if (((S_LEVEL > S_ENDLEVEL)&&(__amplitude > S_ENDLEVEL))||((S_LEVEL < S_ENDLEVEL) && (__amplitude < S_ENDLEVEL))) {
			__amplitude += calculateSlope(S_LEVEL, S_ENDLEVEL,__sustain);
		}
		
		break;
	case 3:

		__amplitude +=calculateSlope(S_LEVEL, 0, __Release);
		if (__amplitude < 0.0) {
			__amplitude = 0.0;
			__state++;
			break;
		}
		break;
	case 4:
		return 0.0;
		break;
	default:
		__state = 0;
		break;
	}
	return __amplitude*__velMulti;
}

void EnvelopeGenerator::RenderBlock(double * buffer, size_t size)
{

	int attackTime = (int)(*__a_time * __sampleRate);
	int holdTime = (int)(*__h_time * __sampleRate);
	int decayTime = (int)(*__d_time * __sampleRate);
	int releaseTime =(int)(*__r_time * __sampleRate);
	int sustainTime =(int)(*__s_time * __sampleRate);

	double a_level = *__a_level * __velMulti;
	double a_c = *__a_curve;
	double a_k = (attackTime)*pow(a_level, -1 / a_c);

	double d_level = *__d_level * __velMulti;
	if (d_level > a_level)
		d_level = a_level;
	double d_c = *__d_curve;
	double d_k = (decayTime)*pow(a_level-d_level, -1 / d_c);

	double s_level = *__s_level * __velMulti;
	if (s_level > d_level)
		s_level = d_level;
	double s_c = *__s_curve;
	double s_k = (sustainTime)*pow(d_level-s_level, -1 / s_c);

	double r_c =*__r_curve;
	double r_k = (releaseTime)*pow(d_level, -1 / r_c);


	//Figure out what settings we gonna use if a reset and amplitude is non-zero
	if (__counter == 0 && __state == 0 && __amplitude > 0.001)
	{
		if (__amplitude<a_level)
		{
			__counter = static_cast<int>(a_k / (pow(__amplitude, -1 / a_c)));
		}
		else if (__amplitude == a_level)
		{
			__state = 1; //Hold state
		}
		else if (__amplitude > a_level) 
		{
			__state = 2; //Decay
			a_level = __amplitude;
		}
	}


	if (__state < 4 && __sustain == 0) {
		__state = 4; //Release
		__counter = static_cast<int>(releaseTime - r_k / (pow(__amplitude, -1 / r_c))); //Calculate were to begin in release
	}

	for (size_t i = 0; i < size; i++)
	{
		switch (__state)
		{
			case 0://Attack
				__amplitude = pow(static_cast<double>(__counter) / a_k, a_c);
				if (__counter>=attackTime)
				{
					__counter = 0;
					__state++;
				}
				if (!isfinite(__amplitude)) 
					__amplitude = a_level;
				break;
			case 1://Hold
				__amplitude = a_level;
				if (__counter>= holdTime)
				{
					__counter = 0;
					__state++;
				}
				break;
			case 2://Decay
				__amplitude = pow(-(static_cast<double>(__counter)- decayTime) / d_k, d_c)+d_level;
				if (__counter>= decayTime)
				{
					__counter = 0;
					__state++;
				}
				if (!isfinite(__amplitude))
					__amplitude = d_level;
				break;
			case 3://Sustain
				if (s_level != d_level) 
				{
					__amplitude = pow(-(static_cast<double>(__counter) - sustainTime) / s_k, s_c) + s_level;
					if (__counter >= sustainTime)
						__amplitude = s_level;
				}
				else
					__amplitude = d_level;
				

				if (__amplitude <= 0.0001) {
					__amplitude = 0.0;
					__state = 5;
				}
				break;
			case 4://Release
				__amplitude = pow(-(static_cast<double>(__counter) - releaseTime) / r_k, r_c);
				if (__counter>= releaseTime)
				{
					__counter = 0;
					__state++;
				}
				if (!isfinite(__amplitude))
					__amplitude = 0;
				break;
			case 5:
				__amplitude = 0.0;
				break;
		}
		jassert(isfinite(__amplitude));
		buffer[i] = __amplitude;
		__counter++;
	}
}

void EnvelopeGenerator::setVelocity(uint8 vel)
{
	__vel = vel;
}

void EnvelopeGenerator::setSustain(uint8 sust)
{
	__sustain = sust;
}



void EnvelopeGenerator::Reset(uint8 vel) {
	__state = 0;
	__vel = vel;
	__velMulti = vel / 127.0;
	__counter = 0;
}


double EnvelopeGenerator::calculateSlope(double begin,double end, double time) {
	if (time == 0)
		return 0;
	return (end - begin) / time;
}

void EnvelopeGenerator::recalculateParameters()
{
}

EnvelopeGenerator::EnvelopeGenerator(int ID,double sampleRate,GLOBAL*global):
	IVSTParameters(ID),
	__sampleRate(sampleRate),
	__state(5),
	__amplitude(0),
	__vel(127),
	__velMulti(1.0),
	__counter(0)
{
	Global = global;
	__a_time = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_ATTACK_TIME");
	__a_level = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_ATTACK_LEVEL");
	__a_curve = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_ATTACK_CURVE");
	__h_time = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_HOLD_TIME");
	__d_time = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_DECAY_TIME");
	__d_level = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_DECAY_LEVEL");
	__d_curve = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_DECAY_CURVE");
	__r_time = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_RELEASE_TIME");
	__r_curve = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_RELEASE_CURVE");

	__s_time = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_SUSTAIN_TIME");
	__s_level = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_SUSTAIN_LEVEL");
	__s_curve = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_SUSTAIN_CURVE");
}

EnvelopeGenerator::~EnvelopeGenerator()
{
}

bool EnvelopeGenerator::isActive() const {
	return __state != 5;
}

void EnvelopeGenerator::Stop()
{
	__amplitude = 0.0;
	__sustain = 0;
	__state = 5;
}


void EnvelopeGenerator::RenderImage(int __ID, Image * image, GLOBAL*Global)
{
	int colourId = Global->paramHandler->Get<AudioParameterChoice>(-1, "THEME")->getIndex();
	ThemePicker tp;
	Colour themeColour = tp.getColour(colourId);
	ScopedPointer<Graphics> g = new Graphics(*image);
	g->setImageResamplingQuality(Graphics::ResamplingQuality::highResamplingQuality);
	g->setColour(Swatch::background);
	g->fillAll();
	
	AudioParameterFloat * __a_time, *__a_level, *__a_curve, *__h_time, *__d_time, *__d_level, *__d_curve, *__r_time, *__r_curve, *__s_level, *__s_time, *__s_curve;
	__a_time  = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_ATTACK_TIME");
	__a_level = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_ATTACK_LEVEL");
	__a_curve = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_ATTACK_CURVE");
	__h_time  = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_HOLD_TIME");
	__d_time  = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_DECAY_TIME");
	__d_level = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_DECAY_LEVEL");
	__d_curve = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_DECAY_CURVE");
	__r_time  = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_RELEASE_TIME");
	__r_curve = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_RELEASE_CURVE");

	__s_time  = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_SUSTAIN_TIME");
	__s_level = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_SUSTAIN_LEVEL");
	__s_curve = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_SUSTAIN_CURVE");
	float __s_time_s = *__s_time;
	double a_level = *__a_level;
	double a_c = *__a_curve;
	

	double d_level = *__d_level;
	if (d_level > a_level)
		d_level = a_level;
	double d_c = *__d_curve;
	

	double s_level = *__s_level;
	if (s_level > d_level)
		s_level = d_level;
	double s_c = *__s_curve;
	

	double r_c = *__r_curve;
	

	if (s_level >= d_level)
	{
		__s_time_s = static_cast<float>((*__a_time + *__h_time + *__d_time + *__r_time)*0.1);
	}
	int __state = 0;
	double __amplitude = 0.0;
	int __imageHeight = image->getHeight();
	int __counter = 0;
	int __sampleRate = static_cast<int>((image->getWidth())/(*__a_time+ *__h_time + *__d_time + *__r_time + __s_time_s));

	int attackTime = (int)(*__a_time * __sampleRate);
	int holdTime = (int)(*__h_time * __sampleRate);
	int decayTime = (int)(*__d_time * __sampleRate);
	int releaseTime = (int)(*__r_time * __sampleRate);
	int sustainTime = (int)(__s_time_s * __sampleRate);
	double a_k = (attackTime)*pow(a_level, -1 / a_c);
	double d_k = (decayTime)*pow(a_level - d_level, -1 / d_c);
	double s_k = (sustainTime)*pow(d_level - s_level, -1 / s_c);
	double r_k = (releaseTime)*pow(d_level, -1 / r_c);
	

	

	Path p = Path();
	p.startNewSubPath(0, image->getHeight());

	g->setColour(themeColour);
	float lastx = 0,lasty = 0;
	for (int i = 4; i < image->getWidth()-6; i++)
	{
		switch (__state)
		{
		case 0://Attack
			__amplitude = pow(static_cast<double>(__counter) / a_k, a_c);
			if (__counter >= attackTime)
			{
				__counter = 0;
				__state++;
			}
			break;
		case 1://Hold
			__amplitude = a_level;
			if (__counter >= holdTime)
			{
				__counter = 0;
				__state++;
			}
			break;
		case 2://Decay
			__amplitude = pow(-(static_cast<double>(__counter) - decayTime) / d_k, d_c) + d_level;
			if (__counter >= decayTime)
			{
				__counter = 0;
				__state++;
			}
			break;
		case 3://Sustain
			if (s_level != d_level)
			{
				__amplitude = pow(-(static_cast<double>(__counter) - sustainTime) / s_k, s_c) + s_level;
				if (__counter >= sustainTime)
					__amplitude = s_level;
			}
			else
				__amplitude = d_level;


			if (__counter > sustainTime) {
				__counter = 0;
				__state = 4;
			}
			break;
		case 4://Release
			__amplitude = pow(-(static_cast<double>(__counter) - releaseTime) / r_k, r_c);
			if (__counter >= releaseTime)
			{
				__counter = 0;
				__state++;
			}
			break;
		case 5:
			__amplitude = 0.0;
			break;
		}
		
		if (std::isfinite(static_cast<float>(__amplitude))) {
			if (i > 4) {
				//g->drawLine(lastx, lasty, static_cast<float>(i), __imageHeight - static_cast<float>(__amplitude*(__imageHeight - 16)) - 6, 6.0f);
				//p.addLineSegment(Line<float>(lastx, lasty, static_cast<float>(i), __imageHeight - static_cast<float>(__amplitude*(__imageHeight - 16)) - 6), 10.0f);
				p.lineTo(static_cast<float>(i), __imageHeight - static_cast<float>(__amplitude*(__imageHeight - 24)) -8 );
			}
			lastx = static_cast<float>(i);
			lasty = __imageHeight - static_cast<float>(__amplitude*(__imageHeight - 16))-6;
			
		}
		__counter++;
	}
	p.lineTo(image->getWidth(), image->getHeight());
	p.closeSubPath();
	//p.addLineSegment(Line<float>(image->getWidth(), image->getHeight(), 0, image->getHeight()), 1.0f);
	p = p.createPathWithRoundedCorners(12.0f);
	g->strokePath(p, PathStrokeType(6.0f,PathStrokeType::JointStyle::curved,PathStrokeType::EndCapStyle::rounded));
	
	Colour startGradient = themeColour.darker(0.3f).withAlpha(0.6f);
	Colour endGradient = themeColour.darker(0.3f).withAlpha(0.05f);
	
	g->setFillType(FillType(ColourGradient(startGradient, 0, 0, endGradient, 0, image->getHeight(), false)));

	//g->setFillType(FillType(ColourGradient(Colour::fromRGBA(26, 105, 180, 80), 0, 0, Colour::fromRGBA(26, 105, 180, 10), 0, image->getHeight(), false)));
	g->fillPath(p);


	int sumX = 6;
	Font f(g->getCurrentFont());
	f.setHeight(f.getHeight() * 1.5);
	g->setFont(f);
	g->setColour(Colour::fromRGBA(150, 150, 255, 15));
	for (size_t i = 1; i <= 10; i++)
	{
		g->drawLine((image->getWidth()/10)*i, 0, (image->getWidth() / 10)*i, image->getHeight());
	}
	for (size_t i = 1; i <= 10; i++)
	{
		g->drawLine(0,(image->getHeight() / 10)*i,image->getWidth(), (image->getHeight() / 10)*i );
	}
	g->setColour(Colour::fromRGBA(255, 255, 255, 40));
	const float dashes[2] = { 30.0f,20.0f };
	if (attackTime > 0) {
		if (g->getCurrentFont().getStringWidth("ATTACK") < attackTime) {
			g->drawText("ATTACK", sumX, image->getHeight() - g->getCurrentFont().getHeight() - 12, attackTime, g->getCurrentFont().getHeight(), Justification::centred, false);
		}
		else if (g->getCurrentFont().getStringWidth("A") < attackTime) {
			g->drawText("A", sumX, image->getHeight() - g->getCurrentFont().getHeight() - 12, attackTime, g->getCurrentFont().getHeight(), Justification::centred, false);
		}
		sumX += attackTime;
		g->drawDashedLine(Line<float>(sumX, 0, sumX, image->getHeight()), dashes, 2);
		//g->drawLine(sumX, 0, sumX, image->getHeight());

	}
	if (holdTime > 0) {
		if (g->getCurrentFont().getStringWidth("HOLD") < holdTime) {
			g->drawText("HOLD", sumX, image->getHeight() - g->getCurrentFont().getHeight() - 12, holdTime, g->getCurrentFont().getHeight(), Justification::centred, false);
		}
		else if (g->getCurrentFont().getStringWidth("H") < holdTime) {
			g->drawText("H", sumX, image->getHeight() - g->getCurrentFont().getHeight() - 12, holdTime, g->getCurrentFont().getHeight(), Justification::centred, false);
		}

		sumX += holdTime;
		g->drawDashedLine(Line<float>(sumX, 0, sumX, image->getHeight()), dashes, 2);

	}
	if (decayTime > 0) {
		if (g->getCurrentFont().getStringWidth("DECAY") < decayTime) {
			g->drawText("DECAY", sumX, image->getHeight() - g->getCurrentFont().getHeight() - 12, decayTime, g->getCurrentFont().getHeight(), Justification::centred, false);
		}
		else if (g->getCurrentFont().getStringWidth("D") < decayTime) {
			g->drawText("D", sumX, image->getHeight() - g->getCurrentFont().getHeight() - 12, decayTime, g->getCurrentFont().getHeight(), Justification::centred, false);
		}
		sumX += decayTime;
		g->drawDashedLine(Line<float>(sumX, 0, sumX, image->getHeight()), dashes, 2);

	}
	if (sustainTime > 0) {
		if (g->getCurrentFont().getStringWidth("SUSTAIN") < sustainTime) {
			g->drawText("SUSTAIN", sumX, image->getHeight() - g->getCurrentFont().getHeight() - 12, sustainTime, g->getCurrentFont().getHeight(), Justification::centred, false);
		}
		else if (g->getCurrentFont().getStringWidth("S") < sustainTime) {
			g->drawText("S", sumX, image->getHeight() - g->getCurrentFont().getHeight() - 12, sustainTime, g->getCurrentFont().getHeight(), Justification::centred, false);
		}
		sumX += sustainTime;
		g->drawDashedLine(Line<float>(sumX, 0, sumX, image->getHeight()), dashes, 2);

	}
	if (releaseTime > 0)
	{
		if (g->getCurrentFont().getStringWidth("RELEASE") < releaseTime) {
			g->drawText("RELEASE", sumX, image->getHeight() - g->getCurrentFont().getHeight() - 12, releaseTime, g->getCurrentFont().getHeight(), Justification::centred, false);
		}
		else if (g->getCurrentFont().getStringWidth("R") < releaseTime) {
			g->drawText("R", sumX, image->getHeight() - g->getCurrentFont().getHeight() - 12, releaseTime, g->getCurrentFont().getHeight(), Justification::centred, false);
		}
	}


	g->setColour(Colour::fromRGB(26, 26, 26));
	g->drawRect(0, 0, image->getWidth(), image->getHeight(), 6);

	
}

void EnvelopeGenerator::RegisterParameters(int ID,GLOBAL*Global)
{
	Global->paramHandler->RegisterFloat(ID, "ENV_ATTACK_LEVEL", "Attack level", 0.01f, 1.0f, 1.0f);
	Global->paramHandler->RegisterFloat(ID, "ENV_ATTACK_TIME", "Attack time", 0.0f, 2.0f, 0.2f);
	Global->paramHandler->RegisterFloat(ID, "ENV_ATTACK_CURVE", "Attack curve", 0.01f, 10.0f, 2.0f);

	Global->paramHandler->RegisterFloat(ID, "ENV_HOLD_TIME", "Hold time", 0.0f, 2.0f, 0.0f);

	Global->paramHandler->RegisterFloat(ID, "ENV_DECAY_LEVEL", "Decay level", 0.01f, 1.0f, 0.9f);
	Global->paramHandler->RegisterFloat(ID, "ENV_DECAY_TIME", "Decay time", 0.0f, 2.0f, 0.2f);
	Global->paramHandler->RegisterFloat(ID, "ENV_DECAY_CURVE", "Decay curve", 0.01f, 10.0f, 1.0f);

	Global->paramHandler->RegisterFloat(ID, "ENV_SUSTAIN_LEVEL", "Sustain level", 0.0f, 1.0f, 1.0f);
	Global->paramHandler->RegisterFloat(ID, "ENV_SUSTAIN_TIME", "Sustain time", 0.01f, 10.0f, 2.0f);
	Global->paramHandler->RegisterFloat(ID, "ENV_SUSTAIN_CURVE", "Sustain curve", 0.01f, 10.0f, 1.0f);

	Global->paramHandler->RegisterFloat(ID, "ENV_RELEASE_TIME", "Release time", 0.0f, 2.0f, 0.2f);
	Global->paramHandler->RegisterFloat(ID, "ENV_RELEASE_CURVE", "Release curve", 0.01f, 10.0f, 1.0f);

}
