#include "FilterHP.h"
#include "FilterButterworth.h"
#include "TemplateHelper.h"
template<typename T>
FilterHP<T>::FilterHP(int ID, double sampleRate, String parameterId) :
	FilterButterworth(ID, sampleRate, parameterId)
{
	
}

template<typename T>
FilterHP<T>::~FilterHP()
{

}
template<typename T>
void FilterHP<T>::RegisterParameters(int ID)
{
	FilterButterworth<T>::RegisterParameters(ID, "HP Frequency", "FILTER_HP", 21.0f);
}
template<typename T>
void FilterHP<T>::CalculateCoefficients()
{
	// Calculate coefficients based on the current cut-off frequency
	T wc = 2 * Constants<T>::PI * __fc;
	wc = 2 * __fs * tan(wc / (2 * __fs));
	T fs2 = __fs * __fs;
	T wc2 = wc * wc;

	__a[0] = 4 * (fs2) + 2 * __fs*wc*__sqrt2 + (wc2);
	__a[1] = (2 * (wc2) - 8 * (fs2)) / __a[0];
	__a[2] = ((wc2) - 2 * __sqrt2*wc*__fs + 4 * (fs2)) / __a[0];

	__b[0] = (4 * fs2) / __a[0];
	__b[1] = __b[0] * -2;
	__b[2] = __b[0];
}

template<typename T>
bool FilterHP<T>::IsEnabled()
{
	// Disable under 20Hz
	return (__fc >= __lowerLimit) && (__fc < __upperLimit);
}

template class FilterHP<double>;
template class FilterHP<float>;