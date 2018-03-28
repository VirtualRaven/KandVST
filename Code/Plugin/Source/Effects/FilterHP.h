#ifndef FILTER_HP_H
#define FILTER_HP_H
#include "IEffect.h"
#include "FilterButterworth.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterHandler.h"

template<typename T>
class FilterHP :
	public FilterButterworth<T>
{

private:
	void CalculateCoefficients() override;
	bool IsEnabled() override;
	
public:
	FilterHP(int ID, double sampleRate, String parameterId,GLOBAL*global);
	~FilterHP();

	static void RegisterParameters(int ID, GLOBAL*Global);
	GLOBAL * Global;

};


#endif //FILTER_HP_H
