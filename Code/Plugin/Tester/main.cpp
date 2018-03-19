#include <Windows.h>
#include <iostream>
#include <string>
#include "base/ipluginbase.h"
#include "folders.h"
#include <vst/ivstaudioprocessor.h>
#include <functional>

HANDLE ourConsole;

void red(std::function<void()> f) {
	if (ourConsole != NULL) {
		SetConsoleTextAttribute(ourConsole, FOREGROUND_RED);
		f();
		SetConsoleTextAttribute(ourConsole, FOREGROUND_BLUE | FOREGROUND_GREEN |  FOREGROUND_RED);
	}
	else f();
}


class dll {
	HMODULE __hdll;
	DWORD __err_code;
public:
	dll(LPCTSTR file) : __hdll(LoadLibrary(file)) {
			__err_code = (__hdll == NULL) ?  GetLastError() : 0;
			if (isLoaded()) {
				std::cout << "Loaded DLL: " << LIB_PATH << std::endl;
			}
	}
	~dll() {
		FreeLibrary(__hdll);
	}
	bool isLoaded() {
		return __hdll != NULL;
	}

	FARPROC getPtr(LPCSTR name) {
		if (isLoaded()) {
			auto tmp = GetProcAddress(__hdll,name);
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
	dll __dll;
	typedef bool(*DLL_FUNC)();
	DLL_FUNC __init;
	DLL_FUNC __exit;
protected:
	Steinberg::IPluginFactory* __factory;
	bool __init_success;

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
			for (Steinberg::int32 i= 0; i < __factory->countClasses(); i++) {
				Steinberg::PClassInfo2 info;
				__factory->getClassInfo(i,&info);
				std::cout << i << "\n\tCID: " << std::hex << info.cid << std::dec << "\n\tName: " << std::string(info.name)<< "\n\tCategory: " << std::string(info.category)<< std::endl << std::endl;
			}
			std::cout << std::endl;
		}

		void printInfo() {
			using Steinberg::PFactoryInfo;
			Steinberg::PFactoryInfo info;
			__factory->getFactoryInfo(&info);
			std::cout << "### Plugin info ###" << std::endl;
			std::cout << "Vendor:" << info.vendor << std::endl;
			std::cout << "Email: " << info.email << std::endl;
			std::cout << "Flags: \n"
				<< "\tkClassesDiscardable:"		<< (PFactoryInfo::kClassesDiscardable & info.flags ? "true" : "false")		<< std::endl
				<< "\tkLicenseCheck:"			<< (PFactoryInfo::kLicenseCheck & info.flags ? "true" : "false")			<< std::endl
				<< "\tkComponentNonDiscardable:"<< (PFactoryInfo::kComponentNonDiscardable & info.flags ? "true" : "false") << std::endl
				<< "\tkUnicode:"				<< (PFactoryInfo::kUnicode & info.flags ? "true" : "false") << std::endl;
			std::cout << std::endl;
			
		}
};


class headless_audio_plugin : public factory_plugin {
	Steinberg::Vst::IComponent* component;
public:
	headless_audio_plugin() {
		if (this->isInitialized()) {
			this->printInfo();
			this->printClasses();
			auto tmp = this->__factory->createInstance(,Steinberg::Vst::IComponent::iid, (void**)(&component));
			if (tmp != Steinberg::kResultOk) {
				__init_success = false;
				component = nullptr;
				red([] { std::cerr << "Plugin does not implement the IComponent interface" << std::endl; });
			}
		}
	}
	~headless_audio_plugin() {
		if (component != nullptr) {
			component->release();
		}
	}
};


int  main(){
	ourConsole = GetStdHandle(STD_ERROR_HANDLE);
	if(ourConsole != NULL)
		SetConsoleTextAttribute(ourConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	headless_audio_plugin plug;
	if (!plug.isInitialized()){
		return 42;
	}
	return 0;
}