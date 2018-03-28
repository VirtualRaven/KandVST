#ifndef FILTER_BUTTERWORTH_H
#define FILTER_BUTTERWORTH_H
#include "IEffect.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterHandler.h"
#include "Global.h"
#include "IVSTParameters.h"


// Generic 2:nd order Butterwroth filter class
template<typename T>
class FilterButterworth :
	public IEffect<T>,
	public IVSTParameters
{

private:
	virtual void CalculateCoefficients() = 0;
	virtual bool IsEnabled() = 0;
	bool __enabled;
	int __firstSampleIndex;

	double __prevFc;

	T __prevX1[2] = { 0, 0 };
	T __prevX2[2] = { 0, 0 };

	T __prevY1[2] = { 0, 0 };
	T __prevY2[2] = { 0, 0 };

	T __currentLeft = 0;
	T __currentRight = 0;

protected:
	T __a[3] = { 1, 0, 0 };
	T __b[3] = { 0, 0, 0 };

	float __fc;
	T __fs;
	T __sqrt2;

	float __upperLimit, __lowerLimit;

	AudioParameterFloat* lpFrequency;
	AudioParameterChoice* lfoIndex;
	
public:
	FilterButterworth(int ID, double sampleRate, String parameterId,GLOBAL*global);
	~FilterButterworth();

	static void RegisterParameters(int ID, String parameterLabel, String parameterId, float defaultValue,GLOBAL*Global);
	bool RenderBlock(AudioBuffer<T>& buffer, int len, bool empty) override;
	virtual void ProccessCommand(MidiMessage message) override;
	virtual void Reset() override;
	GLOBAL * Global;

};


#endif //FILTER_BUTTERWORTH_H
