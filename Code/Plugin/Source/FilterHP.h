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
	FilterHP(int ID, double sampleRate, String parameterId);
	~FilterHP();

	static void RegisterParameters(int ID);
};


#endif //FILTER_HP_H
