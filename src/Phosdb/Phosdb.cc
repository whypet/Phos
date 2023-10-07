#include "Phosdb.hh"

VOID
Log(
	IN LOG    Type,
	IN PCWSTR Format,
	...
) {
	va_list Args;
	va_start(Args, Format);

	INT32 Length = _vscwprintf(Format, Args);
	PWSTR Text = static_cast<PWSTR>(HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (Length + 1) * sizeof(WCHAR)));

	vswprintf(Text, Length + 1, Format, Args);

	va_end(Args);

	PCWSTR TypeSz;

	switch (Type) {
	case LOG::Info:
		TypeSz = L"inf";
		break;
	case LOG::Warn:
		TypeSz = L"wrn";
		break;
	case LOG::Error:
		TypeSz = L"err";
		break;
	}

	SYSTEMTIME Time = { 0 };
	GetLocalTime(&Time);

	wprintf(L"[%02d:%02d:%02d] (%s) %s\n", Time.wHour, Time.wMinute, Time.wSecond, TypeSz, Text);

	if (Type == LOG::Error)
		MessageBoxW(NULL, Text, L"Phosdb", MB_ICONERROR);

	HeapFree(GetProcessHeap(), 0, Text);
}