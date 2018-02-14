#ifndef FILTER_BUTTERWORTH_H
#define FILTER_BUTTERWORTH_H
#include "IEffect.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterHandler.h"
#include "Global.h"
#include "IVSTParameters.h"


// Generic 2:nd order Butterwroth filter class
class FilterButterworth :
	public IEffect,
	public IVSTParameters
{

private:
	template<typename T>
	void __RenderBlock(AudioBuffer<T>& buffer);
	virtual void CalculateCoefficients() = 0;

	double __prevFc;

	double __prevX1[2] = { 0, 0 };
	double __prevX2[2] = { 0, 0 };

	double __prevY1[2] = { 0, 0 };
	double __prevY2[2] = { 0, 0 };

	double __currentLeft = 0;
	double __currentRight = 0;

protected:
	double __a[3] = { 1, 0, 0 };
	double __b[3] = { 0, 0, 0 };

	float __fc;
	float __fs;
	double __sqrt2;

	AudioParameterFloat* lpFrequency;
	
public:
	FilterButterworth(int ID, double sampleRate, String parameterLabel, String parameterId);
	~FilterButterworth();

	// Inherited via IEffect
	virtual void RenderBlock(AudioBuffer<float>& buffer) override 
	{
		__RenderBlock(buffer);
	}
	virtual void RenderBlock(AudioBuffer<double>& buffer) override
	{
		__RenderBlock(buffer);
	}
	virtual void ProccessCommand(MidiMessage message) override;
};


#endif //FILTER_BUTTERWORTH_H
