#include "FilterButterworth.h"
#include "LFO.h"

template<typename T>
FilterButterworth<T>::FilterButterworth(int ID, double sampleRate, String parameterId) :
	IEffect(sampleRate),
	IVSTParameters(ID),
	__enabled(true),
	__firstSampleIndex(0),
	__upperLimit(24000.0f),
	__lowerLimit(20.0f)
{
	__fs = sampleRate;
	__prevFc = -1;
	__sqrt2 = sqrt(2);
	lpFrequency = Global->paramHandler->Get<AudioParameterFloat>(ID, parameterId);
	lfoIndex = Global->paramHandler->Get<AudioParameterInt>(ID, parameterId + "_LFO");
}

template<typename T>
FilterButterworth<T>::~FilterButterworth()
{
}
template<typename T>
void FilterButterworth<T>::RegisterParameters(int ID, String parameterLabel, String parameterId, float defaultValue)
{
	Global->paramHandler->RegisterFloat(ID, parameterId, parameterLabel, 21.0f, 23990.0f, defaultValue);
	Global->paramHandler->RegisterInt(ID, parameterId + "_LFO", parameterLabel + " lfo", 0, 2, 0);
}

template<typename T>
bool FilterButterworth<T>::RenderBlock(AudioBuffer<T>& buffer, int len, bool empty)
{
	__fc = *lpFrequency;

	if (!__enabled && ((!empty) && IsEnabled()))
	{
		// Enabled again
		__enabled = true;

		// Set the previous values
		if (len >= 2)
		{
			// This will become y[i-1]
			__currentLeft = buffer.getSample(0, 1);
			__currentRight = buffer.getSample(1, 1);

			for (int i = 0; i < 2; i++)
			{
				__prevX2[i] = 0;
				__prevY2[i] = 0;

				// This will become y[i-2]
				__prevX1[i] = buffer.getSample(i, 0);
				__prevY1[i] = buffer.getSample(i, 0);
			}

			// Make the loop begin with sample 2
			__firstSampleIndex = 2;
		}
		
	}
	else if (!__enabled)
	{
		// Return false if not enabled
		return false;
	}

	// Recalculate coefficients only if fc has changed
	if (__fc != __prevFc)
	{
		CalculateCoefficients();
	}
	__prevFc = __fc;

	auto buff = buffer.getArrayOfWritePointers();
	
	double* lfo = nullptr;
	float amount = 0.0f;
	bool active = false;
	bool tmpbool = (2 * (*lpFrequency) < __upperLimit) && (0.5 * (*lpFrequency) > __lowerLimit);
	if ((*lfoIndex) > 0 && tmpbool) {
		if (lfos[(*lfoIndex) - 1]->isActive()) active = true;;
		static double lastAmount = 0.0;
		lfo = lfos[(*lfoIndex) - 1]->getPointer();
		amount = lfos[(*lfoIndex) - 1]->getAmount();
		amount = (pow(2, amount)*(*lpFrequency) >= __upperLimit) || (pow(2, amount)*(*lpFrequency) <= __lowerLimit) ? lastAmount : amount;
		lastAmount = amount;
	}
		 

	for (int i = __firstSampleIndex; i < len; i++)
	{
		__firstSampleIndex = 0;
		if (active && (*lfoIndex) > 0 && tmpbool) {
			__fc = (*lpFrequency) * pow(2, lfo[i] * amount);
			CalculateCoefficients();
		}
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

	if ((empty || !IsEnabled()) && __enabled)
	{
		__enabled = false;
	}

	return true;
}



template<typename T>
void FilterButterworth<T>::ProccessCommand(MidiMessage message)
{
}

template class FilterButterworth<double>;
template class FilterButterworth<float>;
