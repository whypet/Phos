#include "Util.hh"

namespace Phosdb {
std::wstring Util::Utf8ToWide(
	const std::string &Utf8
) {
	// docs lie about including null terminator in returned length
	INT32 Length = MultiByteToWideChar(CP_UTF8, 0, Utf8.c_str(), Utf8.length(), NULL, 0);
	WCHAR *Wide = new WCHAR[Length + 1];

	MultiByteToWideChar(CP_UTF8, 0, Utf8.c_str(), Utf8.length(), Wide, Length);
	Wide[Length] = L'\0';
	std::wstring String(Wide);

	delete[] Wide;

	return String;
}
}