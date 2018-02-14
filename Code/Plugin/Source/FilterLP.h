#ifndef FILTER_LP_H
#define FILTER_LP_H
#include "IEffect.h"
#include "FilterButterworth.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterHandler.h"


class FilterLP :
	public FilterButterworth
{

private:
	void __calculateCoefficients();
	virtual void CalculateCoefficients() override
	{
		__calculateCoefficients();
	}
	
public:
	FilterLP(double sampleRate, ParameterHandler& paramHandler, String parameterId);
	~FilterLP();
};


#endif //FILTER_LP_H
