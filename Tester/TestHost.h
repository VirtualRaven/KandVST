/*
 * This file is part of the KandVST synthesizer.
 *
 * Copyright (c) 2018   Lukas Rahmn, Anton Fredriksson,
 *                      Sarosh Nasir, Stanisław Zwierzchowski,
 *                      Klas Ludvigsson and Andreas Kuszli
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef TEST_HOST_H
#define TEST_HOST_H
#include "wrapperVST.h"
#include "hostVST.h"
#include <sstream>
#include <string>
#include <map>

class TestHost;

template<typename T,size_t S>
class ProcessBlock {
	std::string testPath;
	friend TestHost;
public:
	static const bool dPrec = std::is_same<T, double>();
	static const size_t size = S;
	T left[S];
	T right[S];
	T* buffs[2] = { left,right };
	bool process(wrapperVST* vst, IParameterChanges* inParams, IEventList* inEvent = nullptr);
	bool processZero(wrapperVST* vst, IParameterChanges* inParams, IEventList* inEvent = nullptr);
	
	//Prints the content of the block to a file. Filename will be prepended with the path to the test folder
	//and appended will be "_1.txt" and "_2.txt" depending on channel.
	//Note that this will automatecaly be called by the test host on the last block, so it is only necessary to call this
	//if the test runs process several times.
	bool printBlock(std::string name);
};

class TestHost {

public:
	static const size_t TEST_BLOCK_SIZE = 44100;
	typedef double TEST_TYPE;
	typedef ProcessBlock<TEST_TYPE, TEST_BLOCK_SIZE> TestBlock;
	
	struct ParamsData {
		std::vector<Steinberg::Vst::ParameterInfo> params;
		std::vector<Steinberg::Vst::ParameterInfo> midi;
		std::map<std::wstring, Steinberg::Vst::ParamID> ids;
	};

	enum TestFlag {
		PrintTCPerf,
		Force
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

	
	void printParams(bool verbose=false,std::string fileName="");
	
	void addTest(Test* test);
	bool runTests();

	void setTestFlag(TestFlag flag, bool value);


private:
	typedef std::tuple<Steinberg::Vst::ParamID, Steinberg::Vst::ParamValue> PARAM_TUP;
	std::vector<PARAM_TUP> readParamFile(std::string filename, bool &sucess);
	std::map<TestFlag, bool> testFlags;
	bool resetParameters();
	bool setParameters(const std::vector<PARAM_TUP>& vals);
	bool runTest(size_t i);
	std::vector<Test*> tests;
	ParamsData pdata;
	wrapperVST vst;
	bool initialized;
};


#endif