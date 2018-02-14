#ifndef FILTER_HP_H
#define FILTER_HP_H
#include "IEffect.h"
#include "FilterButterworth.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterHandler.h"


class FilterHP :
	public FilterButterworth
{

private:
	void __calculateCoefficients();
	virtual void CalculateCoefficients() override
	{
		__calculateCoefficients();
	}
	
public:
	FilterHP(double sampleRate, ParameterHandler& paramHandler, String parameterId);
	~FilterHP();
};


#endif //FILTER_HP_H
