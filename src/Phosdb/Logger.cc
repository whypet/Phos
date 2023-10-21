#include "Logger.hh"

namespace Phosdb {
VOID Logger::Log(
	INT32  Type,
	PCWSTR Scope,
	PCWSTR Format,
	...
) {
#if !DEBUG
	if (Type == Trace)
		return;
#endif

	va_list Args;
	va_start(Args, Format);

	INT32 Length = _vscwprintf(Format, Args);
	PWSTR Text = static_cast<PWSTR>(HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (Length + 1) * sizeof(WCHAR)));

	vswprintf(Text, Length + 1, Format, Args);

	va_end(Args);

	PCWSTR TypeSz;

	switch (Type) {
	case Trace:
		TypeSz = L"dbg";
		break;
	case Info:
		TypeSz = L"inf";
		break;
	case Warn:
		TypeSz = L"wrn";
		break;
	case Error:
		TypeSz = L"err";
		break;
	}

	SYSTEMTIME Time = { 0 };
	GetLocalTime(&Time);

	wprintf(L"[%02d:%02d:%02d] (%s) [%s] %s\n", Time.wHour, Time.wMinute, Time.wSecond, TypeSz, Scope, Text);

	if (Type == Error)
		MessageBoxW(NULL, Text, L"Phosdb", MB_ICONERROR);

	HeapFree(GetProcessHeap(), 0, Text);
}
};