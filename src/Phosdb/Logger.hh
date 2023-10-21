#pragma once

#include "Phosdb.hh"

static std::wstring MakeLogScope(
	const std::wstring &PrettyFunction
) {
	UINTN Position;

	std::wstring Where(PrettyFunction);
	
	Position = Where.find_last_of(L"(");
	if (Position != std::wstring::npos)
		Where = Where.substr(0, Position);
	
	Position = Where.find_first_of(L"::");
	if (Position != std::wstring::npos)
		Where = Where.substr(Position + 2);
	
	return Where;
}

#define LOG(Type, Format, ...) Phosdb::Logger::Log(Phosdb::Logger::Type, MakeLogScope(WIDE_PREFIX(__PRETTY_FUNCTION__)).c_str(), Format, __VA_ARGS__)

namespace Phosdb {
class Logger {
public:
	enum {
		Trace,
		Info,
		Warn,
		Error
	};

	static VOID Log(
		INT32  Type,
		PCWSTR Scope,
		PCWSTR Format,
		...
	);
};
}