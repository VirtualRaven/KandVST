#ifndef IWAVETABLE_H
#define IWAVETABLE_H

class IWavetable
{
protected:
	int __length;
public:
	int getLength() const
	{
		return __length;
	}
	virtual double getSample(int phase, double frequency) const = 0;
	virtual float getSample(int phase, float frequency) const = 0 ;
};

enum WAVE_TYPE : unsigned int {
	SINE = 0,
	SQUARE ,
	SAW ,
	TRI ,
	__COUNT
};

inline WAVE_TYPE toWAVE_TYPE(int a) {
	switch (a)
	{
		case SINE:
			return SINE;
		case SQUARE:
			return SQUARE;
		case SAW:
			return SAW;
		case TRI:
			return TRI;
		default:
			return SINE;
	}
}

extern const IWavetable* tables[WAVE_TYPE::__COUNT];
extern void populateWavetable(double sampleRate);
extern void freeWavetable();
#endif //!IWAVETABLE_H