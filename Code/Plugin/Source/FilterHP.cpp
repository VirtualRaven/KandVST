#include "FilterHP.h"

FilterHP::FilterHP(int ID, double sampleRate, String parameterId) :
	FilterButterworth(ID, sampleRate, "HP Frequency", parameterId)
{
	
}


FilterHP::~FilterHP()
{

}

void FilterHP::__calculateCoefficients()
{
	// Calculate coefficients based on the current cut-off frequency
	double wc = 2 * double_Pi * fc;

	a[0] = 4 * (fs * fs) + 2 * fs*wc*sqrt2 + (wc * wc);
	a[1] = (2 * (wc * wc) - 8 * (fs * fs)) / a[0];
	a[2] = ((wc * wc) - 2 * sqrt2*wc*fs + 4 * (fs * fs)) / a[0];

	b[0] = (4 * fs * fs) / a[0];
	b[1] = b[0] * -2;
	b[2] = b[0];
}
