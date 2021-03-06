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

#include "TestHost.h"
#include "util.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include  <iomanip>
#include <wchar.h>
#include <type_traits>
#include "hostVST.h"
#include <vst/ivstaudioprocessor.h>
#include <fstream>
#include "folders.h"
#include <cstdlib>
#include "Windows.h"

#undef max
#undef min


void printParameter(const Steinberg::Vst::ParameterInfo& i, bool verbose=false, std::wostream &stream = std::wcout) {
	using namespace Steinberg::Vst;



	stream << i.unitId << L" " << std::setfill(L'0') << std::setw(sizeof(decltype(i.id)) * 2) << std::hex << i.id << std::dec << L": " << i.title << L" ";
	
		if (i.stepCount == 0)
			stream << L"R [0,1]"
			<< L" D: " << i.defaultNormalizedValue << std::endl;
		else if (i.stepCount == 1)
			stream << "On/Off D: "
			<< (((int)std::min(1.0, 2 * i.defaultNormalizedValue) == 0) ? L"Off" : L"On") << std::endl;
		else
			stream << L"N [0," << i.stepCount << "] D: "
			<< ((int)std::min(1.0, i.stepCount * i.defaultNormalizedValue) == 0) << std::endl;
		using Steinberg::Vst::ParameterInfo;
		if (verbose) {
#define FLAG_STR(V,F) L"\t\t" << #F << (V & F ? L": true" : L": false")
			stream << L"\tFlags:\n"
			<< L"\t\t" << FLAG_STR(i.flags, ParameterInfo::kCanAutomate) << std::endl
			<< L"\t\t" << FLAG_STR(i.flags, ParameterInfo::kIsBypass) << std::endl
			<< L"\t\t" << FLAG_STR(i.flags, ParameterInfo::kIsList) << std::endl
			<< L"\t\t" << FLAG_STR(i.flags, ParameterInfo::kIsProgramChange) << std::endl
			<< L"\t\t" << FLAG_STR(i.flags, ParameterInfo::kIsReadOnly) << std::endl
			<< L"\t\t" << FLAG_STR(i.flags, ParameterInfo::kIsWrapAround) << std::endl;
#undef FLAG_STR
	}
}



void FetchParams(wrapperVST& vst, TestHost::ParamsData& d) {
	
	#define MIDI_PRE L"MIDI CC"
	//Print info about parameters
	auto count = vst.edit()->getParameterCount();
	for (decltype(count) i = 0; i < count; i++) {
		Steinberg::Vst::ParameterInfo info;
		if (vst.edit()->getParameterInfo(i, info) == Steinberg::kResultOk) {
			size_t len = std::min(sizeof(MIDI_PRE) / sizeof(wchar_t) - 1, wcslen(info.title));
			if (0 != wmemcmp(info.title, MIDI_PRE, len)) {
				d.params.push_back(info);
				d.ids.emplace(std::wstring(info.title), info.id);
			}
			else
				d.midi.push_back(info);

		}
		else util::red([i] {std::cerr << "Parameter " << i << " failed to to be fetched" << std::endl; });
	}
	std::sort(d.params.begin(), d.params.end() - 1,
		[](Steinberg::Vst::ParameterInfo a, Steinberg::Vst::ParameterInfo b) {return wcscmp(a.title, b.title) == -1; });
}

TestHost::TestHost() : initialized(false)
{
	if (!vst.isInitialized()) {
		return;
	}

	FetchParams(vst, pdata);

	Steinberg::Vst::SpeakerArrangement inspeakers = Steinberg::Vst::SpeakerArr::kEmpty;
	Steinberg::Vst::SpeakerArrangement outspeakers = Steinberg::Vst::SpeakerArr::kStereo;
	
	if (vst.proc()->setBusArrangements(&inspeakers, 1, &outspeakers, 1) != Steinberg::kResultTrue) {
		util::red([] {"Failed to set bus arrangments\n"; });
		return;
	}

	//Apparently we force stop input bus
	if (vst.com()->activateBus(Steinberg::Vst::MediaTypes::kAudio,
		Steinberg::Vst::BusDirections::kInput, 0, false) != Steinberg::kResultTrue) {
		util::red([] {"Failed to disable input bus\n"; });
		return;
	}

	Steinberg::Vst::ProcessSetup config = {
		Steinberg::Vst::kRealtime,
		TestHost::TestBlock::dPrec ? Steinberg::Vst::kSample64 : Steinberg::Vst::kSample32, 
		TestHost::TEST_BLOCK_SIZE,
		44100.0	
	};

	if (vst.proc()->setupProcessing(config) != Steinberg::kResultOk) {
	}

	initialized = true;
}

bool TestHost::isInitialized()
{
	return initialized;
}

bool TestHost::resetParameters()
{
	vst.proc()->setProcessing(true);
	ProcessBlock<double,1U> block;
	HParameterChanges changes;
	for (auto p : pdata.params) {
		int32 i;
		auto q = changes.addParameterData(p.id, i);
		q->addPoint(0, p.defaultNormalizedValue, i);
		q->release();
	}

	auto res = block.processZero(&vst, &changes);
	vst.proc()->setProcessing(false);
	return res ;
}

bool TestHost::setParameters(const std::vector<PARAM_TUP>& vals)
{
	vst.proc()->setProcessing(true);
	ProcessBlock<double, 1U> block;
	HParameterChanges changes;
	for (auto p : vals) {
		int32 i;
		auto q = changes.addParameterData(std::get<0>(p), i);
		q->addPoint(0, std::get<1>(p), i);
		q->release();
	}
	auto res = block.processZero(&vst, &changes);
	vst.proc()->setProcessing(false);
	return res ;
}

void TestHost::printParams(bool verbose, std::string filename) {
	if (filename != "")
	{
		std::ifstream f(filename);
		if (!f.good()) {
			f.close();
			std::wofstream file(filename, std::ios_base::trunc);
			for (auto& t : this->pdata.params)
				printParameter(t, verbose,file);
			file.close();
		}
		else
		{
			f.close();
			std::cout << "Can't write file, file already exists" << std::endl;
		}
	}
	else {
		util::cyan([] {std::cout << "### Parameters ###" << std::endl; });
		for (auto& t : this->pdata.params)
			printParameter(t, verbose);
	}

}

void TestHost::addTest(Test * test)
{
	this->tests.push_back(test);
}

bool TestHost::runTests()
{
	bool acc = true;
	auto test_len = tests.size();
	for (size_t i = 0; i < test_len; i++) {
#ifdef NO_PY_FOUND
		if (tests[i]->hasPythonStep()) {
			if (testFlags[Force])
			{
				util::red([&] {std::cout << "Ttest (" << i + 1 << "/" << test_len << ") failed as it requires python." << std::endl; });
				return false;
			}
			util::yellow([&] {std::cout << "Skipping test (" << i+1 <<"/"<< test_len <<") as it requires python."  << std::endl; });
			continue;
		}
#endif

		if (!this->resetParameters()) {
			util::red([&] {std::cerr << "Failed to clear parameters before running test" << i + 1 << "." << std::endl; });
			return false;
		}

		if (this->vst.proc()->setProcessing(true) != Steinberg::kResultOk) {
			util::red([&] {std::cerr << "Failed to start processing before running test" << i + 1 << "." << std::endl; });
			return false;
		}
		bool tmp = this->runTest(i);
		if (this->vst.proc()->setProcessing(false) != Steinberg::kResultOk) {
			util::red([&] {std::cerr << "Failed to stop processing after running test" << i + 1 << "." << std::endl; });
			return false;
		}
		if (tmp)
			util::cyan([&] {std::cout << "Test (" << i+1 << "/" << test_len << ") was successful" << std::endl; });
		else
			util::red([&] {std::cout << "Test (" << i+1 << "/" << test_len << ") failed" << std::endl; });
		acc &= tmp;

	}
	return acc;
}

void TestHost::setTestFlag(TestFlag flag, bool value)
{
	testFlags[flag] = value;
}

std::wstring trim(const std::wstring& str) {
	auto begin = str.find_first_not_of(L" \t");
	if (begin == std::wstring::npos)
		return L"";
	auto end = str.find_last_not_of(L" \t");
	return str.substr(begin, end - begin + 1);
}

enum PTYPE {
	UNK,
	ONOFF, //Needed change, strange interaction with Windows.h
	DISC,
	CONT
};

std::vector<TestHost::PARAM_TUP> TestHost::readParamFile(std::string filename,bool& sucess)
{
	std::vector<PARAM_TUP> val;
	std::wifstream file;
	file.open(filename);
	if (!file.is_open()) {
		sucess = false;
		return val;
	}
	std::wstring str;
	int line = 0;
	bool unormalised = true;
	while (++line,std::getline(file, str)) {
		
		//Ignore comment
		{
			auto res = trim(str);
			if (str[0] == L'#') {
				if (str == L"#NORMALIZED")
					unormalised = false;
				else if (str == L"#UNNORMALIZED")
					unormalised = true;
				continue;
			}
			else if (str.size() == 0)
				continue;
		}
		//No comment, parse the line
		auto res = str.find(L"=");
		if (res == std::wstring::npos) {
			util::red([&] {std::cerr << "Syntax error in file " << filename << " at line " << line << std::endl << "Expected equality sign." << std::endl ; });
			sucess = false;
			return val;
		}
		if (res + 1 >= str.size()) {
			util::red([&] {std::cerr << "Syntax error in file " << filename << " at line " << line << std::endl << "Expected value after equality sign." << std::endl; });
			sucess = false;
			return val;
		}
		std::wstring name = trim(str.substr(0, res));
		std::wstring value = trim(str.substr(res + 1));
		double fvalue = 0.0;

		auto search = this->pdata.ids.find(name);
		if (search != this->pdata.ids.end()) {
			PTYPE t = UNK;
			Steinberg::int32 stepCount = 0;
			if (unormalised) {
				for (auto p : this->pdata.params) {
					if (p.id == search->second) {
						if (p.stepCount == 0)
							t = CONT;
						else if (p.stepCount == 1)
							t = ONOFF;
						else if (p.stepCount > 1)
							t = DISC;
						stepCount = p.stepCount;
					}
				}
			}
			else {
				t = CONT;
			}
			if (t== UNK) {
				util::red([&] {std::cerr << "Program error in file " << filename << " at line " << line << std::endl; std::wcerr << name << L"Unknown parameter name" << std::endl; });
			}

			switch (t) {
				case CONT:
					try{
						if (value.find(L'.') == std::string::npos)
							throw std::invalid_argument("Floating point value must contain a dot");
						fvalue = std::stod(value);
						if (fvalue > 1.0 || fvalue < 0) {
							util::red([&] {std::cerr << "Value error in file " << filename << " at line " << line << std::endl << "A value after a floating point parameter must be between 0.0 and 1.0" << std::endl; });
							sucess = false;
							return val;
						}
					}
					catch (std::invalid_argument& ) {
						util::red([&] {std::cerr << "Syntax error in file " << filename << " at line " << line << std::endl << "Value after equality sign is not an valid floating point number." << std::endl; });
						sucess = false;
						return val;
					}

					break;
				case ONOFF:
					if (value == L"On" || value == L"True")
						fvalue = 1.0;
					else if (value == L"Off" || value == L"False")
						fvalue = 0.0;
					else {
						util::red([&] {std::cerr << "Syntax error in file " << filename << " at line " << line << std::endl << "Parameter is of type bool, expected On, True, Off or False" << std::endl; });
						sucess = false;
						return val;
					}
					break;
				case DISC:
					try {
						auto tmp = std::stoi(value);
						if (tmp < 0) {
							util::red([&] {std::cerr << "Syntax error in file " << filename << " at line " << line << std::endl << "Parameter is of type integer, expected value larger than zero" << std::endl; });
							sucess = false;
							return val;
						}
						else if (tmp > stepCount) {
							util::red([&] {std::cerr << "Syntax error in file " << filename << " at line " << line << std::endl << "Value to large, Parameter is of type integer, with max size "<< stepCount << std::endl; });
							sucess = false;
							return val;
						}
						fvalue = tmp / stepCount;
					}
					catch (std::invalid_argument&) {
						util::red([&] {std::cerr << "Syntax error in file " << filename << " at line " << line << std::endl << "Value after equality sign is not an valid integer." << std::endl; });
						sucess = false;
						return val;
					}
					break;
			}
			val.push_back(std::make_tuple(search->second, fvalue));


		}
		else {
			util::red([&] {std::cerr << "Program error in file " << filename << " at line " << line << std::endl; std::wcerr << name << L" is not a valid parameter name." << std::endl; });
			sucess = false;
			return val;

		}
	}

	if (line > 0) {
		sucess = true;
		return val;
	}
	sucess = false;
	return val;

}

bool TestHost::runTest(size_t i)
{
	//Setup pref. monitor
	auto h = GetCurrentProcess();
	ULONG64 pre = 0, post = 0, diff = -1;
	bool perfSucc = true;

	const auto count = tests.size();
	auto& test = tests[i];
	const std::string testPath = std::string(TEST_BUILD_PATH) + std::string(test->name()) + std::string("/");
	test->block.testPath = testPath;
	std::string testName = test->name();
	util::cyan([&] {std::cout << "Running test (" << (i+1) << "/" << count << ") " << testName << std::endl; });
	if (test->hasParameterFile()) {
		std::string path = std::string(TEST_PATH) + testName + std::string("/params.txt");
		bool s=false;
		auto res = this->readParamFile(path, s);
		if (s) {
			if (!this->setParameters(res)) {
				util::red([&] {std::cout << "Test " << testName << " failed" << std::endl << "Could not set parameters"<< std::endl; });
				return false;
			}
		}
		else {
			util::red([&] {std::cout << "Test " << testName << " failed" << std::endl << "Could not read test parameters. " << std::endl; });
			return false;
		}
	}
	std::stringstream ss;
	perfSucc &= QueryProcessCycleTime(h, &pre);
	bool testValid = test->run(&this->vst, ss, this->pdata);
	perfSucc &= QueryProcessCycleTime(h, &post);
	if (testValid)
	{
		diff = post - pre;
	}
	std::ofstream perf(testPath + std::string("perf.txt"), std::ios_base::trunc);
	perf << diff;
	if (testFlags[PrintTCPerf])
	{
		std::cout << "##teamcity[buildStatisticValue key = '"<< testName << "_perf' value = '"<< diff << "']" << std::endl;
	}

	if (!testValid) {
		
		util::red([&] {std::cout << "Test process" << testName << " failed" << std::endl; });
		return false;
	}
	auto test_msg = ss.str();
	if (test_msg.size() > 0) {
		std::ofstream log(testPath + std::string("log.txt"), std::ios_base::trunc);
		if (log.is_open()) {
			log << test_msg;
			log.close();
		}
		else {
			util::red([&] {std::cout << "Test " << testName << " failed" << std::endl << "Could not write test log. " << std::endl; });
			return false;
		}
	}

	if (test->exportTestData()) {
		if(test->block.printBlock("data")){
			if (test->hasPythonStep() ) {
				const std::string testPyPath = std::string(TEST_PATH) + std::string(test->name())+ std::string("/verify.py");
				//Execute python on test data
				int ret = system((std::string(PY_PATH) +std::string(" ")+ testPyPath +  std::string(" ") + testPath).c_str());
				if (ret == 42)
					return true;
				else {
					util::red([&] {std::cout << "Test " << testName << " failed" << std::endl << "Python verification signaled error" << std::endl; });
					return false;
				}
			}
		}
		else {
			util::red([&] {std::cout << "Test " << testName << " failed" << std::endl << "Could not write test data. " << std::endl; });
			return false;
		}

	}
	else if (test->hasPythonStep()) {
		util::red([&] {std::cout << "Test " << testName << " is erroneously written, if it has python step it must export data." << std::endl; });
		return false;
	}

	return true;

}

template<typename T, size_t S>
bool ProcessBlock<T, S>::process(wrapperVST* vst, IParameterChanges* inParams, IEventList* inEvent) {
	Steinberg::Vst::ProcessData data;
	data.processMode = Steinberg::Vst::kRealtime;
	data.symbolicSampleSize = dPrec ? Steinberg::Vst::kSample64 : Steinberg::Vst::kSample32;
	data.numSamples = size;
	data.numInputs = 0;
	data.numOutputs = 1;
	data.inputs = nullptr;
	Steinberg::Vst::AudioBusBuffers buffOut;
	buffOut.numChannels = 2;
	buffOut.silenceFlags = 0;
	ptrHelper<T>(buffOut) = buffs;
	data.outputs = &buffOut;
	data.inputParameterChanges = inParams;
	data.outputParameterChanges = nullptr;
	data.inputEvents = inEvent;
	data.outputEvents = nullptr;
	data.processContext = nullptr;

	return vst->proc()->process(data) == Steinberg::kResultOk;
}

template<typename T, size_t S>
bool ProcessBlock<T, S>::processZero(wrapperVST* vst, IParameterChanges* inParams, IEventList* inEvent) {
	Steinberg::Vst::ProcessData data;
	data.processMode = Steinberg::Vst::kRealtime;
	data.symbolicSampleSize = dPrec ? Steinberg::Vst::kSample64 : Steinberg::Vst::kSample32;
	data.numSamples = 0;
	data.numInputs = 0;
	data.numOutputs = 1;
	data.inputs = nullptr;
	Steinberg::Vst::AudioBusBuffers buffOut;
	buffOut.numChannels = 2;
	buffOut.silenceFlags = 0;
	ptrHelper<T>(buffOut) = buffs;
	data.outputs = &buffOut;
	data.inputParameterChanges = inParams;
	data.outputParameterChanges = nullptr;
	data.inputEvents = inEvent;
	data.outputEvents = nullptr;
	data.processContext = nullptr;

	return vst->proc()->process(data) == Steinberg::kResultOk;
}

template<typename T, size_t S>
bool ProcessBlock<T, S>::printBlock(std::string name)
{
		//Export the test data
		std::ofstream data1(testPath +  name + std::string("_1.txt"),std::ios_base::trunc);
		std::ofstream data2(testPath +  name + std::string("_2.txt"),std::ios_base::trunc);
		if (data1.is_open() && data2.is_open()) {
			for (size_t i = 0; i < S - 1; i++) {
				data1 << left[i] << ',';
				data2 << right[i] << ',';
			}
			data1 << left[S - 1];
			data2 << right[S - 1];
			data1.close();
			data2.close();
			return true;
		}
		else
			return false;
	
}

template class ProcessBlock<TestHost::TEST_TYPE, TestHost::TEST_BLOCK_SIZE>;

