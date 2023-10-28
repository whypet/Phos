#include <Phosdef.h>

#define LOG(Type, Format, ...) \
	Log(Type, __PRETTY_FUNCTION__, Format, ##__VA_ARGS__)

enum {
	Trace,
	Info,
	Warn,
	Error
};

EXTERN_C VOID Log(
	INT32       Type,
	const CHAR *PrettyFunc,
	const CHAR *Format,
	...
);