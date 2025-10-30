#pragma once
#include <type_traits>
#include <format>
#include <iostream>
#include <Defines.h>
#include <variant>
#include <cassert>

template <class str>
class DebugView
{
public:
	virtual const str& ToString(int fmt) = 0;
};

enum Severity { 
	Error = ConsoleColour::RED, 
	Warn = ConsoleColour::YELLOW, 
	Info = ConsoleColour::GREEN
};

namespace Matrix::Filesys {
	class File;
}

template <class str>
class Debugger {
public:
	enum OutMethod {
		FILE,
		STD_COUT,
	};
	Debugger() = default;
	Debugger(const OutMethod _out_method)
		: out_method(_out_method) {
		ostr = "";
	}

	Debugger(const OutMethod _out_method, Matrix::Filesys::File* _file)
		: out_method(_out_method), file(_file) {
		ostr = "";
	}
	template <class T>
	Debugger<str>& LogError(str msg, T* obj = nullptr, int fmt=0);
	template <class T>
	Debugger<str>& LogWarn(str msg, T* obj = nullptr, int fmt=0);
	template <class T>
	Debugger<str>& LogInfo(str msg, T* obj = nullptr, int fmt=0);
	void flush() {
		switch (out_method) {
		case (FILE):
			assert(file != nullptr);
			file->Write(ostr.c_str(), ostr.size());
			break;
		case (STD_COUT):
			std::cout << ostr;
			break;
		}
	}
private:
	const OutMethod out_method;
	std::string ostr;
	Matrix::Filesys::File* file = nullptr;
	template <class T>
	Debugger<str>& Log(Severity Severity, str msg, T* obj = nullptr, int fmt=0);
};

using Logger = Debugger<std::string>;

static Logger Debug = Logger(Logger::STD_COUT);