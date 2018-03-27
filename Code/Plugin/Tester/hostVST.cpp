#include "hostVST.h"
#include "hostVST.h"

using namespace Steinberg;
using namespace Steinberg::Vst;

#include <string>

int32	HEventList::getEventCount() {
	return __vec.size();
}

tresult HEventList::getEvent(int32 index, Event & e)
{
	const auto& tmp =__vec[index];
	memcpy(&e, &tmp, sizeof(Event));
	return Steinberg::kResultOk;
}

tresult HEventList::addEvent(Event & e)
{
	__vec.emplace_back();
	memcpy(&__vec.back(),&e,sizeof(Event));
	return Steinberg::kResultOk;
}


tresult HEventList::queryInterface(const TUID _iid, void **obj) {
		QUERY_INTERFACE(_iid, obj, Steinberg::FUnknown::iid, Steinberg::FUnknown)
		QUERY_INTERFACE(_iid, obj, Steinberg::Vst::IEventList::iid, Steinberg::Vst::IEventList);
		*obj = nullptr;
		return Steinberg::kNoInterface;
}
uint32 	HEventList::addRef() {
	return ++__ref;
}
uint32 HEventList::release() {
	if (__ref == 0)
		return 0;
	else {
		if (--__ref == 0)
			delete this;
		return __ref;
	}
}

HParameterChanges::~HParameterChanges()
{
	for (auto& t : __vec) {
		t->release();
		t = nullptr;
	}
}

int32 HParameterChanges::getParameterCount()
{
	return __vec.size();
}

/*****************************************************
 *
 * HParameterChanges
 * 
*****************************************************/
Steinberg::Vst::IParamValueQueue * HParameterChanges::getParameterData(int32 index) {
	auto tmp = __vec[index];
	tmp->addRef();
	return tmp;
}

IParamValueQueue * 	HParameterChanges::addParameterData(const Steinberg::Vst::ParamID &id, int32 &index) {
	for (auto& t : __vec)
		if (t->getParameterId() == id)
			return nullptr;

	auto tmp = new HParamValueQueue(id);
	__vec.push_back(tmp);
	index = __vec.size() - 1;
	tmp->addRef();
	return tmp;
}

//FUnknown
tresult HParameterChanges::queryInterface(const TUID _iid, void **obj) {
	QUERY_INTERFACE(_iid, obj, Steinberg::FUnknown::iid, Steinberg::FUnknown)
	QUERY_INTERFACE(_iid, obj, Steinberg::Vst::IParameterChanges::iid, Steinberg::Vst::IParameterChanges);
	*obj = nullptr;
	return Steinberg::kNoInterface;
}
uint32 	HParameterChanges::addRef() {
	return ++__ref;
}
uint32 HParameterChanges::release() {
	if (__ref == 0)
		return 0;
	else {
		if (--__ref == 0)
			delete this;
		return __ref;
	}

}


/*****************************************************
 *
 * HParamValueQueue
 * 
*****************************************************/

ParamID 	HParamValueQueue::getParameterId() {
	return __id;
}

int32 HParamValueQueue::getPointCount() {
	return __vec.size();
}
		
tresult 	HParamValueQueue::getPoint(int32 index, int32 &sampleOffset, ParamValue &value) {
	if (index >= __vec.size()) {
		return Steinberg::kInvalidArgument;
	}
	auto tmp = __vec[index];
	sampleOffset = std::get<0>(tmp);
	value = std::get<1>(tmp);
	return Steinberg::kResultOk;
}

tresult 	HParamValueQueue::addPoint(int32 sampleOffset, ParamValue value, int32 &index) {
	__vec.push_back(std::make_tuple(sampleOffset, value));
	return __vec.size() - 1;
}
		
		//FUnknown
		tresult HParamValueQueue::queryInterface(const TUID _iid, void **obj) {
			QUERY_INTERFACE(_iid, obj, Steinberg::FUnknown::iid, Steinberg::FUnknown)
			QUERY_INTERFACE(_iid, obj, Steinberg::Vst::IParamValueQueue::iid, Steinberg::Vst::IParamValueQueue);
			*obj = nullptr;
			return Steinberg::kNoInterface;
		}
		uint32 	HParamValueQueue::addRef() {
			return ++__ref;
		}
		uint32 HParamValueQueue::release() {
			if (__ref == 0)
				return 0;
			else {
		if (--__ref == 0)
			delete this;
		return __ref;
	}
		}



template<> double**& ptrHelper<double>(Steinberg::Vst::AudioBusBuffers& buff) {
	return buff.channelBuffers64;
}
template<> float**& ptrHelper<float>(Steinberg::Vst::AudioBusBuffers& buff) {
	return buff.channelBuffers32;
}