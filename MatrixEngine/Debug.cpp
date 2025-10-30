#include "Debug.hpp"

template<class str> template<class T>
inline Debugger<str>& Debugger<str>::Log(Severity Severity, str msg, T* obj, int fmt)
{
	if (std::is_convertible<T, DebugView<str>>()) {
		DebugView<str>* view = obj;
		if (obj != nullptr)
			ostr << std::format("{0}[{1}]: {2} :: {3}", ColourCodes[Severity], Severity, msg, view->ToString(fmt));
		else
			ostr << std::format("{0}[{1}]: {2}", ColourCodes[Severity], Severity, msg);
	}
	fchain
}

template<class str> template<class T>
Debugger<str>& Debugger<str>::LogError(str msg, T* obj, int fmt)
{
	return Log(Severity::Error, msg, obj);
}

template<class str> template<class T>
Debugger<str>& Debugger<str>::LogWarn(str msg, T* obj, int fmt)
{
	return Log(Severity::Warn, msg, obj);
}

template<class str> template<class T>
Debugger<str>& Debugger<str>::LogInfo(str msg, T* obj, int fmt)
{
	return Log(Severity::Info, msg, obj);
}
