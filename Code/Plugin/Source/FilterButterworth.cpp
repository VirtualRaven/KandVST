#include "FilterButterworth.h"

FilterButterworth::FilterButterworth(int ID, double sampleRate, String parameterId) :
	IEffect(sampleRate),
	IVSTParameters(ID)
{
	__fs = sampleRate;
	__prevFc = -1;
	__sqrt2 = sqrt(2);
	lpFrequency = Global->paramHandler->Get<AudioParameterFloat>(ID, parameterId);
}


FilterButterworth::~FilterButterworth()
{
}

void FilterButterworth::RegisterParameters(int ID, String parameterLabel, String parameterId)
{
	Global->paramHandler->RegisterFloat(ID, parameterId, parameterLabel, 1.0f, 20000.0f, 400.0f);
}

template<typename T>
void FilterButterworth::__RenderBlock(AudioBuffer<T>& buffer)
{
	__fc = *lpFrequency;

	// Recalculate coefficients only if fc has changed
	if (__fc != __prevFc)
	{
		CalculateCoefficients();
	}
	__prevFc = __fc;

	auto buff = buffer.getArrayOfWritePointers();
	for (int i = 0; i < buffer.getNumSamples(); i++)
	{
		// Current y[i-2] is the previous y[i-1]
		__prevY2[0] = __prevY1[0];
		__prevY2[1] = __prevY1[1];

		// Current y[i-1] is the previous currentLeft/Right
		__prevY1[0] = __currentLeft;
		__prevY1[1] = __currentRight;

		__currentLeft = buffer.getSample(0, i);
		__currentRight = buffer.getSample(1, i);

		__currentLeft = __b[0] * __currentLeft + __b[1] * __prevX1[0] + __b[2] * __prevX2[0] 
					  - __a[1] * __prevY1[0] - __a[2] * __prevY2[0];
		__currentRight = __b[0] * __currentLeft + __b[1] * __prevX1[1] + __b[2] * __prevX2[1] 
					   - __a[1] * __prevY1[1] - __a[2] * __prevY2[1];

		// Next x[i-2] will be the current x[i-1]
		__prevX2[0] = __prevX1[0];
		__prevX2[1] = __prevX1[1];

		// Next x[i-1] will be the current x[i]
		__prevX1[0] = buffer.getSample(0, i);
		__prevX1[1] = buffer.getSample(1, i);

		// Change the sample
		buff[0][i]=  static_cast<T>(__currentLeft);
		buff[1][i] = static_cast<T>(__currentLeft);

	}
}

template void FilterButterworth::__RenderBlock(AudioBuffer<double>& buffer);
template void FilterButterworth::__RenderBlock(AudioBuffer<float>& buffer);

void FilterButterworth::ProccessCommand(MidiMessage message)
{
}
