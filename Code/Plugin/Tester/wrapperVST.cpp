//This is to force the VST api to initialize IID
//we need this as we only use the interface headers
#define INIT_CLASS_IID

#include <Windows.h>
#include "wrapperVST.h"
#include <iostream>
#include <string>
#include "base/ipluginbase.h"
#include "base/funknown.h"
#include "folders.h"
#include <vst/ivstaudioprocessor.h>
#include <vst/ivsthostapplication.h>
#include <functional>
#include <vector>
#include "util.h"
#define APP_NAME u"AUTOMATIC TESTER"

using util::red;

struct STUID {
	Steinberg::TUID id;

	STUID(Steinberg::TUID __id) {
		memcpy(id, __id, sizeof(Steinberg::TUID));
	}
};




class WinDLL {
	HMODULE __hdll;
	DWORD __err_code;
public:
	WinDLL(LPCTSTR file) : __hdll(LoadLibrary(file)) {
		__err_code = (__hdll == NULL) ? GetLastError() : 0;
		if (isLoaded()) {
			std::cout << "Loaded DLL: " << LIB_PATH << std::endl;
		}
	}
	WinDLL(const WinDLL&) = delete;

	~WinDLL() {
		FreeLibrary(__hdll);
	}
	bool isLoaded() {
		return __hdll != NULL;
	}

	FARPROC getPtr(LPCSTR name) {
		if (isLoaded()) {
			auto tmp = GetProcAddress(__hdll, name);
			if (tmp == NULL) {
				__err_code = GetLastError();
			}
			else {
				return tmp;
			}
		}
		return nullptr;
	}

	DWORD getErrCode() {
		return __err_code;
	}


};

class factory_plugin {
	WinDLL __dll;
	typedef bool(*DLL_FUNC)();
	DLL_FUNC __init;
	DLL_FUNC __exit;
protected:
	Steinberg::IPluginFactory* __factory;
	bool __init_success;
	std::vector<STUID> __ids;

public:
	factory_plugin() : __dll(LIB_PATH),
		__init(nullptr),
		__exit(nullptr),
		__factory(nullptr),
		__init_success(false) {
		if (__dll.isLoaded()) {

			__init = reinterpret_cast<DLL_FUNC>(__dll.getPtr("InitDll"));
			if (__init == nullptr) {
				red([] {std::cerr << "Failed to find symbol InitDll" << std::endl; });
				return;
			}

			__exit = reinterpret_cast<DLL_FUNC>(__dll.getPtr("ExitDll"));
			if (__exit == nullptr) {
				red([] {std::cerr << "Failed to find symbol ExitDll" << std::endl; });
				return;
			}

			if (__init()) {
				GetFactoryProc fproc = reinterpret_cast<GetFactoryProc>(__dll.getPtr("GetPluginFactory"));
				__factory = fproc ? fproc() : nullptr;
				if (__factory != nullptr) {
					__init_success = true;
					for (Steinberg::int32 i = 0; i < __factory->countClasses(); i++) {
						Steinberg::PClassInfo info;
						__factory->getClassInfo(i, &info);
						__ids.emplace_back(info.cid);
					}
				}
			}

		}
		else
			red([] {std::cerr << "Failed to load plugin " << LIB_PATH << std::endl; });
	}
	~factory_plugin() {
		if (__factory != nullptr)
			__factory->release();
		if (__exit != nullptr)
			__exit();
	}

	bool isInitialized() {
		return __init_success;
	}

	void printClasses() {
		std::cout << "### Classes ###" << std::endl;
		for (Steinberg::int32 i = 0; i < __factory->countClasses(); i++) {
			Steinberg::PClassInfo info;
			__factory->getClassInfo(i, &info);
			std::cout << i << "\n\tCID: " << std::hex << info.cid << std::dec << "\n\tName: " << std::string(info.name) << "\n\tCategory: " << std::string(info.category) << std::endl << std::endl;
		}
		std::cout << std::endl;
	}

	template<typename T> T* findInterface() {
		for (const auto& id : __ids) {
			T* obj = nullptr;
			Steinberg::tresult res = this->__factory->createInstance(id.id,
				T::iid, (void**)(&obj));
			if (res == Steinberg::kResultOk && obj != nullptr)
				return obj;
			else continue;
		}
		return nullptr;
	}

	void printInfo() {
		using Steinberg::PFactoryInfo;
		Steinberg::PFactoryInfo info;
		__factory->getFactoryInfo(&info);
		std::cout << "### Plugin info ###" << std::endl;
		std::cout << "Vendor:" << info.vendor << std::endl;
		std::cout << "Email: " << info.email << std::endl;
		std::cout << "Flags: \n"
			<< "\tkClassesDiscardable:" << (PFactoryInfo::kClassesDiscardable & info.flags ? "true" : "false") << std::endl
			<< "\tkLicenseCheck:" << (PFactoryInfo::kLicenseCheck & info.flags ? "true" : "false") << std::endl
			<< "\tkComponentNonDiscardable:" << (PFactoryInfo::kComponentNonDiscardable & info.flags ? "true" : "false") << std::endl
			<< "\tkUnicode:" << (PFactoryInfo::kUnicode & info.flags ? "true" : "false") << std::endl;
		std::cout << std::endl;

	}
};

using Steinberg::Vst::IAudioProcessor;

class audioPlugin : public factory_plugin {
	IAudioProcessor* __pro;
public:
	audioPlugin() : __pro(nullptr) {
		if (this->isInitialized()) {
			this->printInfo();
			this->printClasses();

			__pro = this->findInterface<IAudioProcessor>();
			if (__pro == nullptr) {
				__init_success = false;
				red([] { std::cerr << "Plugin does not implement the IAudioProcessor interface" << std::endl; });
			}
		}
	}

	IAudioProcessor* operator->() {
		return __pro;
	}

	virtual ~audioPlugin() {
		if (__pro != nullptr) {
			__pro->release();
		}
	}
};


IAudioProcessor * wrapperVST::operator->() {
	return __plug->operator->();
}

bool wrapperVST::isInitialized() {
	return __plug->isInitialized();
}

wrapperVST::wrapperVST() : __plug(new audioPlugin()) {

}


wrapperVST::~wrapperVST() {
	delete __plug;
	__plug = nullptr;
}

using Steinberg::TUID;

class TesterHost : public Steinberg::Vst::IHostApplication {
	size_t __refs;
public:
	TesterHost() : __refs(1) {}
	virtual Steinberg::tresult PLUGIN_API getName(Steinberg::Vst::String128  name) override {
		memcpy(name, APP_NAME, sizeof(APP_NAME));
		return Steinberg::kResultOk;
	}
	virtual Steinberg::tresult PLUGIN_API createInstance(TUID cid, TUID _iid, void ** obj) {
		return Steinberg::kResultFalse;
	}

	virtual Steinberg::tresult PLUGIN_API queryInterface(const TUID _iid, void **obj) override {
		QUERY_INTERFACE(iid, obj, Steinberg::FUnknown::iid, Steinberg::FUnknown)
			QUERY_INTERFACE(iid, obj, Steinberg::Vst::IHostApplication::iid, Steinberg::Vst::IHostApplication);
		*obj = nullptr;
		return Steinberg::kNoInterface;
	}

	virtual Steinberg::uint32 PLUGIN_API addRef() override {
		return ++__refs;
	}
	virtual Steinberg::uint32 PLUGIN_API release() override {
		return __refs = __refs > 0 ? --__refs : 0;
	}
};


