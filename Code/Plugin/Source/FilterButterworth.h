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

	double prevFc;

	double prevX1[2] = { 0, 0 };
	double prevX2[2] = { 0, 0 };

	double prevY1[2] = { 0, 0 };
	double prevY2[2] = { 0, 0 };

	double currentLeft = 0;
	double currentRight = 0;

protected:
	double a[3] = { 1, 0, 0 };
	double b[3] = { 0, 0, 0 };

	float fc;
	float fs;
	double sqrt2;

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
