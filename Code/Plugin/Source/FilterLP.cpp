#include "FilterLP.h"
#include "FilterButterworth.h"
#include "TemplateHelper.h"

template<typename T>
FilterLP<T>::FilterLP(int ID, double sampleRate, String parameterId) :
	FilterButterworth(ID, sampleRate, parameterId)
{
	
}

template<typename T>
FilterLP<T>::~FilterLP()
{

}
template<typename T>
void FilterLP<T>::RegisterParameters(int ID)
{
	FilterButterworth<T>::RegisterParameters(ID, "LP Frequency", "FILTER_LP", 20000.0f);
}

template<typename T>
void FilterLP<T>::CalculateCoefficients()
{
	// Calculate coefficients based on the current cut-off frequency
	T wc = 2 * Constants<T>::PI * __fc;
	wc = 2 * __fs * tan(wc / (2 * __fs));
	T fs2 = __fs * __fs;
	T wc2 = wc * wc;

	__a[0] = 4 * (fs2) + 2 * __fs*wc*__sqrt2 + (wc2);
	__a[1] = (2 * (wc2) - 8 * (fs2)) / __a[0];
	__a[2] = ((wc2) - 2 * __sqrt2*wc*__fs + 4 * (fs2)) / __a[0];

	__b[0] = (wc2) / __a[0];
	T gain = (1 + __a[1] + __a[2]) / (__b[0] * 3);
	__b[0] *= gain;
	__b[1] = __b[0] * 2;
	__b[2] = __b[0];
}
template<typename T>
bool FilterLP<T>::IsEnabled()
{
	// Disable at 20kHz
	return (__fc < __upperLimit) && (__fc > __lowerLimit);
}

template class FilterLP<double>;
template class FilterLP<float>;