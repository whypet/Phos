#include "Logger.hh"

#include <Util.hh>

extern "C" VOID Log(
	INT32       Type,
	const CHAR *Format,
	...
) {
	va_list Args;
	va_start(Args, Format);

	Phosdb::Logger::Log(Type, MakeLogScope(__PRETTY_FUNCTION__).c_str(), Format, Args);

	va_end(Args);
}

namespace Phosdb {
VOID Logger::Log(
	INT32       Type,
	const CHAR *Scope,
	const CHAR *Format,
	...
) {
	va_list Args;
	va_start(Args, Format);

	Log(Type, Scope, Format, Args);

	va_end(Args);
}

VOID Logger::Log(
	INT32       Type,
	const CHAR *Scope,
	const CHAR *Format,
	va_list     Args
) {
#ifndef DEBUG
	if (Type == Trace)
		return;
#endif
	INT32 Length = _vscprintf(Format, Args);
	CHAR *Text = new CHAR[Length + 1];

	vsnprintf(Text, Length + 1, Format, Args);

	const CHAR *TypeString;

	switch (Type) {
	case Trace:
		TypeString = "dbg";
		break;
	case Info:
		TypeString = "inf";
		break;
	case Warn:
		TypeString = "wrn";
		break;
	case Error:
		TypeString = "err";
		break;
	}

	SYSTEMTIME Time = { 0 };
	GetLocalTime(&Time);

	printf("[%02d:%02d:%02d] (%s) [%s] %s\n", Time.wHour, Time.wMinute, Time.wSecond, TypeString, Scope, Text);

	if (Type == Error) {
		MessageBoxW(
			NULL,
			Util::Utf8ToWide(std::string(Text)).c_str(),
			std::format(L"Phosdb ({})", Util::Utf8ToWide(std::string(Scope))).c_str(),
			MB_ICONERROR);
	}

	delete[] Text;
}
};