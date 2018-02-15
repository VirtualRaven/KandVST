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
	FilterLP(int ID, double sampleRate, String parameterId);
	~FilterLP();

	static void RegisterParameters(int ID);

};


#endif //FILTER_LP_H
