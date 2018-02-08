#ifndef I_VST_PARAMETERS_H
#define I_VST_PARAMETERS_H

class IVSTParameters
{
private:
		IVSTParameters();
protected:
	int __ID;
public:
	IVSTParameters(int ID) : __ID(ID){};

};

#endif // !I_VST_PARAMETERS_H
