#include "TestHost.h"
#include "TestHost.h"
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


void printParameter(const Steinberg::Vst::ParameterInfo& i, bool verbose=false) {
	using namespace Steinberg::Vst;
	std::wcout << i.unitId << L" " << std::setfill(L'0') << std::setw(sizeof(decltype(i.id)) * 2) << std::hex << i.id << std::dec << L": " << i.title << L" ";
	
		if (i.stepCount == 0)
			std::cout << "R [0,1]"
			<<  " D: " << i.defaultNormalizedValue << std::endl;
		else if (i.stepCount == 1)
			std::cout << "On/Off D: "
			<< (((int)std::min(1.0, 2 * i.defaultNormalizedValue) == 0) ? "Off" : "On") << std::endl;
		else
			std::cout << "N [0," << i.stepCount << "] D: "
			<< ((int)std::min(1.0, i.stepCount * i.defaultNormalizedValue) == 0) << std::endl;
		using Steinberg::Vst::ParameterInfo;
		if (verbose) {
#define FLAG_STR(V,F) "\t\t" << #F << (V & F ? ": true" : ": false")
		std::cout << "\tFlags:\n"
			<< "\t\t" << FLAG_STR(i.flags, ParameterInfo::kCanAutomate) << std::endl
			<< "\t\t" << FLAG_STR(i.flags, ParameterInfo::kIsBypass) << std::endl
			<< "\t\t" << FLAG_STR(i.flags, ParameterInfo::kIsList) << std::endl
			<< "\t\t" << FLAG_STR(i.flags, ParameterInfo::kIsProgramChange) << std::endl
			<< "\t\t" << FLAG_STR(i.flags, ParameterInfo::kIsReadOnly) << std::endl
			<< "\t\t" << FLAG_STR(i.flags, ParameterInfo::kIsWrapAround) << std::endl;
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
	vst.proc()->setProcessing(true);
		HParameterChanges c;
		ProcessBlock<double,1024> block;
		block.process(&vst, &c);

	vst.proc()->setProcessing(false);
}

bool TestHost::isInitialized()
{
	return initialized;
}

bool TestHost::resetParameters()
{
	vst.proc()->setProcessing(true);
	TestBlock block;
	HParameterChanges changes;
	for (auto p : pdata.params) {
		int32 i;
		auto q = changes.addParameterData(p.id, i);
		q->addPoint(0, p.defaultNormalizedValue, i);
		q->release();
	}

	auto res = block.process(&vst, &changes);
	vst.proc()->setProcessing(false);
	return res ;
}

bool TestHost::setParameters(const std::vector<PARAM_TUP>& vals)
{
	vst.proc()->setProcessing(true);
	TestBlock block;
	HParameterChanges changes;
	for (auto p : vals) {
		int32 i;
		auto q = changes.addParameterData(std::get<0>(p), i);
		q->addPoint(0, std::get<1>(p), i);
		q->release();
	}
	auto res = block.process(&vst, &changes);
	vst.proc()->setProcessing(false);
	return res ;
}

void TestHost::printParams(bool verbose) {
	util::cyan([] {std::cout << "### Parameters ###" << std::endl; });
	for(auto& t : this->pdata.params)
		printParameter(t, verbose);
}

void TestHost::addTest(Test * test)
{
	this->tests.push_back(test);
}

bool TestHost::runTests()
{
	bool acc = true;
	for (size_t i = 0; i < tests.size(); i++) {
		if (!this->resetParameters()) {
			util::red([&] {std::cerr << "Failed to clear parameters before runing test" << i + 1 << "." << std::endl; });
			return false;
		}
		acc &= this->runTest(i);

	}
	return acc;
}

std::wstring trim(const std::wstring& str) {
	auto begin = str.find_first_not_of(L" \t");
	if (begin == std::wstring::npos)
		return L"";
	auto end = str.find_last_not_of(L" \t");
	return str.substr(begin, end - begin + 1);
}

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
	while (++line,std::getline(file, str)) {
		//Parse the line
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
		try{
			fvalue = std::stod(value);
		}
		catch (std::invalid_argument& arg) {
				util::red([&] {std::cerr << "Syntax error in file " << filename << " at line " << line << std::endl << "Value after equality sign is not an valid floating point number." << std::endl; });
				sucess = false;
				return val;
		}
		auto search = this->pdata.ids.find(name);
		if (search != this->pdata.ids.end())
			val.push_back(std::make_tuple(search->second, fvalue));
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

}

bool TestHost::runTest(size_t i)
{
	const auto count = tests.size();
	auto& test = tests[i];
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
			util::red([&] {std::cout << "Test " << testName << " failed" << std::endl; });
			return false;
		}
	}
	std::stringstream ss;
	if (!test->run(&this->vst, ss, this->pdata)) {
		
		util::red([&] {std::cout << "Test process" << testName << " failed with message: " << ss.str() << std::endl; });
		return false;
	}

	if (test->exportTestData()) {
		//Export the test data
		if (test->hasPythonStep()) {
			//Execute python on test data
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

template class ProcessBlock<TestHost::TEST_TYPE, TestHost::TEST_BLOCK_SIZE>;

