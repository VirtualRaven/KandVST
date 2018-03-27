#ifndef FILTER_LP_H
#define FILTER_LP_H
#include "IEffect.h"
#include "FilterButterworth.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterHandler.h"

template<typename T>
class FilterLP :
	public FilterButterworth<T>
{

private:
	void CalculateCoefficients() override;
	bool IsEnabled() override;
	
public:
	FilterLP(int ID, double sampleRate, String parameterId,GLOBAL*global);
	~FilterLP();

	static void RegisterParameters(int ID, GLOBAL*Global);
	GLOBAL * Global;

};


#endif //FILTER_LP_H
