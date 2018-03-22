#include "FilterLP.h"
#include "FilterButterworth.h"
#include "TemplateHelper.h"

template<typename T>
FilterLP<T>::FilterLP(int ID, double sampleRate, String parameterId) :
	FilterButterworth<T>(ID, sampleRate, parameterId)
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
	T wc = 2 * Constants<T>::PI * this->__fc;
	wc = 2 * this->__fs * tan(wc / (2 * this->__fs));
	T fs2 = this->__fs * this->__fs;
	T wc2 = wc * wc;

	this->__a[0] = 4 * (fs2) + 2 * this->__fs*wc*this->__sqrt2 + (wc2);
	this->__a[1] = (2 * (wc2) - 8 * (fs2)) / this->__a[0];
	this->__a[2] = ((wc2) - 2 * this->__sqrt2*wc*this->__fs + 4 * (fs2)) / this->__a[0];

	this->__b[0] = (wc2) / this->__a[0];
	T gain = (1 + this->__a[1] + this->__a[2]) / (this->__b[0] * 3);
	this->__b[0] *= gain;
	this->__b[1] = this->__b[0] * 2;
	this->__b[2] = this->__b[0];
}
template<typename T>
bool FilterLP<T>::IsEnabled()
{
	// Disable at 20kHz
	return (this->__fc < __upperLimit) && (this->__fc >= __lowerLimit);
}

template class FilterLP<double>;
template class FilterLP<float>;
