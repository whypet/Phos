#include "Logger.hh"
#include "Util.hh"

namespace Phosdb {
VOID Logger::Log(
	INT32       Type,
	const CHAR *Scope,
	const CHAR *Format,
	...
) {
#if !DEBUG
	if (Type == Trace)
		return;
#endif

	va_list Args;
	va_start(Args, Format);

	INT32 Length = _vscprintf(Format, Args);
	CHAR *Text = new CHAR[Length + 1];

	vsnprintf(Text, Length + 1, Format, Args);

	va_end(Args);

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