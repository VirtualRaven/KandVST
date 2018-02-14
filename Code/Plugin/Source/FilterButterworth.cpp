#include "FilterButterworth.h"

FilterButterworth::FilterButterworth(int ID, double sampleRate, String parameterLabel, String parameterId) :
	IEffect(sampleRate),
	IVSTParameters(ID)
{
	fs = sampleRate;
	prevFc = -1;
	sqrt2 = sqrt(2);
	lpFrequency = Global.paramHandler->Get<AudioParameterFloat>(ID, parameterId);
	Global.paramHandler->RegisterFloat(ID, parameterId, parameterLabel, 1.0f, 20000.0f, 400.0f);
}


FilterButterworth::~FilterButterworth()
{
}

template<typename T>
void FilterButterworth::__RenderBlock(AudioBuffer<T>& buffer)
{
	fc = *lpFrequency;

	// Recalculate coefficients only if fc has changed
	if (fc != prevFc)
	{
		CalculateCoefficients();
	}
	prevFc = fc;

	for (size_t i = 0; i < buffer.getNumSamples(); i++)
	{
		// Current y[i-2] is the previous y[i-1]
		prevY2[0] = prevY1[0];
		prevY2[1] = prevY1[1];

		// Current y[i-1] is the previous currentLeft/Right
		prevY1[0] = currentLeft;
		prevY1[1] = currentRight;

		currentLeft = buffer.getSample(0, i);
		currentRight = buffer.getSample(1, i);

		currentLeft = b[0] * currentLeft + b[1] * prevX1[0] + b[2] * prevX2[0] - a[1] * prevY1[0] - a[2] * prevY2[0];
		currentRight = b[0] * currentLeft + b[1] * prevX1[1] + b[2] * prevX2[1] - a[1] * prevY1[1] - a[2] * prevY2[1];

		// Next x[i-2] will be the current x[i-1]
		prevX2[0] = prevX1[0];
		prevX2[1] = prevX1[1];

		// Next x[i-1] will be the current x[i]
		prevX1[0] = buffer.getSample(0, i);
		prevX1[1] = buffer.getSample(1, i);

		// Change the sample
		buffer.setSample(0, i, currentLeft);
		buffer.setSample(1, i, currentLeft);

	}
}

template void FilterButterworth::__RenderBlock(AudioBuffer<double>& buffer);
template void FilterButterworth::__RenderBlock(AudioBuffer<float>& buffer);

void FilterButterworth::ProccessCommand(MidiMessage message)
{
}
