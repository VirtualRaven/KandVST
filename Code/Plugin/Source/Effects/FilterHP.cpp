#include "FilterHP.h"
#include "FilterButterworth.h"
#include "TemplateHelper.h"
template<typename T>
FilterHP<T>::FilterHP(int ID, double sampleRate, String parameterId,GLOBAL*global) :
	FilterButterworth<T>(ID, sampleRate, parameterId,global)
{
	Global = global;
}

template<typename T>
FilterHP<T>::~FilterHP()
{

}
template<typename T>
void FilterHP<T>::RegisterParameters(int ID,GLOBAL*Global)
{
	FilterButterworth<T>::RegisterParameters(ID, "HP Frequency", "FILTER_HP", 20.0f,Global);
}
template<typename T>
void FilterHP<T>::CalculateCoefficients()
{
	// Calculate coefficients based on the current cut-off frequency
	T wc = 2 * Constants<T>::PI * this->__fc;
	wc = 2 * this->__fs * tan(wc / (2 * this->__fs));
	T fs2 = this->__fs * this->__fs;
	T wc2 = wc * wc;

	this->__a[0] = 4 * (fs2) + 2 * this->__fs*wc*this->__sqrt2 + (wc2);
	this->__a[1] = (2 * (wc2) - 8 * (fs2)) / this->__a[0];
	this->__a[2] = ((wc2) - 2 * this->__sqrt2*wc*this->__fs + 4 * (fs2)) / this->__a[0];

	this->__b[0] = (4 * fs2) / this->__a[0];
	this->__b[1] = this->__b[0] * -2;
	this->__b[2] = this->__b[0];
}

template<typename T>
bool FilterHP<T>::IsEnabled()
{
	// Disable under 20Hz
	return this->__fc >= __lowerLimit;
}

template class FilterHP<double>;
template class FilterHP<float>;
