#ifndef TEST_HOST_H
#define TEST_HOST_H
#include "wrapperVST.h"
#include "hostVST.h"
#include <sstream>
#include <string>
#include <map>
template<typename T,size_t S>
class ProcessBlock {

public:
	static const bool dPrec = std::is_same<T, double>();
	static const size_t size = S;
	T left[S];
	T right[S];
	T* buffs[2] = { left,right };
	bool process(wrapperVST* vst, IParameterChanges* inParams, IEventList* inEvent = nullptr);
};

class TestHost {

public:
	static const size_t TEST_BLOCK_SIZE = 1024;
	typedef double TEST_TYPE;
	typedef ProcessBlock<TEST_TYPE, TEST_BLOCK_SIZE> TestBlock;
	
	struct ParamsData {
		std::vector<Steinberg::Vst::ParameterInfo> params;
		std::vector<Steinberg::Vst::ParameterInfo> midi;
		std::map<std::wstring, Steinberg::Vst::ParamID> ids;
	};


	class Test {
	public:
		TestBlock block;
			//Returns the name of the test
			//Must be a valid folder name and same name as the class
			virtual const char * name() const = 0;

			//Runs the test, returns true if it succeeded runing the test
			virtual bool run(wrapperVST* vst,std::stringstream& msg, const TestHost::ParamsData& d) = 0;
			
			//Check if block data should be exported
			//It will be exported to PATH_TO_BUILD/Tests/YOUR_TEST_NAME/test_data.bin
			virtual bool exportTestData() = 0;
			
			//Check if test has external python step to be run
			//The python script should be named verify.py and be located in Tests/YOUR_TEST_NAME/verify.py
			virtual bool hasPythonStep() = 0;
			
			//Check if test has parameter files
			//If it does, the parameters will initialized to this befor calling run
			//The parameter file should be named params.txt and be located in Tests/YOUR_TEST_NAME/params.txt
			virtual bool hasParameterFile() = 0;
	};


	TestHost();
	bool isInitialized();

	
	void printParams(bool verbose=false);
	
	void addTest(Test* test);
	bool runTests();

private:
	typedef std::tuple<Steinberg::Vst::ParamID, Steinberg::Vst::ParamValue> PARAM_TUP;
	std::vector<PARAM_TUP> readParamFile(std::string filename, bool &sucess);
	
	bool resetParameters();
	bool setParameters(const std::vector<PARAM_TUP>& vals);
	bool runTest(size_t i);
	std::vector<Test*> tests;
	ParamsData pdata;
	wrapperVST vst;
	bool initialized;
};


#endif