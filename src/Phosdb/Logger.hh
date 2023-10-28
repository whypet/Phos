#pragma once

#include <Phosdb.hh>

#define LOG(Type, Format, ...) \
	Phosdb::Logger::Log(Phosdb::Logger::Type, MakeLogScope(__PRETTY_FUNCTION__).c_str(), Format, ##__VA_ARGS__)

[[maybe_unused]] static std::string MakeLogScope(
	const std::string &PrettyFunction
) {
	UINTN Position;
	std::string Where(PrettyFunction);
	
	Position = Where.find_last_of("(");
	if (Position != std::string::npos)
		Where = Where.substr(0, Position);
	
	Position = Where.find_first_of("::");
	if (Position != std::string::npos) {
		Position = Where.substr(0, Position).find_last_of(" ");
		
		if (Position != std::string::npos)
			Where = Where.substr(Position + 1);
	}
	
	return Where;
}

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
		INT32       Type,
		const CHAR *Scope,
		const CHAR *Format,
		...
	);

	static VOID Log(
		INT32       Type,
		const CHAR *Scope,
		const CHAR *Format,
		va_list     Args
	);
};
}