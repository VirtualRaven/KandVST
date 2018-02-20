#ifndef IWAVETABLE_H
#define IWAVETABLE_H

class IWavetable
{
protected:
	static const int __length = 2048;
	static const int __NrTables = 10;
	double __tables[__NrTables][__length];
public:
	int getLength() const
	{
		return __length;
	}
	virtual double getSample(double phase, double frequency) const = 0;
	virtual float getSample(double phase, float frequency) const = 0 ;
	


	struct tableSampleLocation{
		int tableNr;
		double diff;
		int i_1;
		int i_2;
	};

	template<bool B> friend  double getSampleFromLoc(const tableSampleLocation& t, const IWavetable* w);

	inline static tableSampleLocation getLoc(double idx, double freq) {
		int tableNr = freq < 20 ? 0 : floor(log2(freq / 20));
		if (tableNr > __NrTables) tableNr = __NrTables - 1;
		double diff = floor(idx) - idx;
		int i = static_cast<int>(idx);
		return { tableNr,
				diff,
				i%__length, 
				i%__length 
		};
	}
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