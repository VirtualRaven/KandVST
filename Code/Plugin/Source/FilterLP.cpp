#include "FilterLP.h"

FilterLP::FilterLP(int ID, double sampleRate, String parameterId) :
	FilterButterworth(ID, sampleRate, "LP Frequency", parameterId)
{
	
}


FilterLP::~FilterLP()
{

}

void FilterLP::__calculateCoefficients()
{
	// Calculate coefficients based on the current cut-off frequency
	double wc = 2 * double_Pi * fc;

	a[0] = 4 * (fs * fs) + 2 * fs*wc*sqrt2 + (wc * wc);
	a[1] = (2 * (wc * wc) - 8 * (fs * fs)) / a[0];
	a[2] = ((wc * wc) - 2 * sqrt2*wc*fs + 4 * (fs * fs)) / a[0];

	b[0] = (wc * wc) / a[0];
	double gain = (1 + a[1] + a[2]) / (b[0] * 3);
	b[0] *= gain;
	b[1] = b[0] * 2;
	b[2] = b[0];
}
