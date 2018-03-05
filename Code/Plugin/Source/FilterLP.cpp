#include "FilterLP.h"
#include "FilterButterworth.h"

FilterLP::FilterLP(int ID, double sampleRate, String parameterId) :
	FilterButterworth(ID, sampleRate, parameterId)
{
	
}


FilterLP::~FilterLP()
{

}

void FilterLP::RegisterParameters(int ID)
{
	FilterButterworth::RegisterParameters(ID, "LP Frequency", "FILTER_LP", 20000.0f);
}

void FilterLP::__calculateCoefficients()
{
	// Calculate coefficients based on the current cut-off frequency
	double wc = 2 * double_Pi * __fc;
	wc = 2 * __fs * tan(wc / (2 * __fs));
	double fs2 = __fs * __fs;
	double wc2 = wc * wc;

	__a[0] = 4 * (fs2) + 2 * __fs*wc*__sqrt2 + (wc2);
	__a[1] = (2 * (wc2) - 8 * (fs2)) / __a[0];
	__a[2] = ((wc2) - 2 * __sqrt2*wc*__fs + 4 * (fs2)) / __a[0];

	__b[0] = (wc2) / __a[0];
	double gain = (1 + __a[1] + __a[2]) / (__b[0] * 3);
	__b[0] *= gain;
	__b[1] = __b[0] * 2;
	__b[2] = __b[0];
}

bool FilterLP::__isEnabled()
{
	// Disable at 20kHz
	return (__fc < 20000.0f) && (__fc > 0.0f);
}
