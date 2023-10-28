#include <Phosdef.h>

#define LOG Log

enum {
	Trace,
	Info,
	Warn,
	Error
};

EXTERN_C VOID Log(
	INT32       Type,
	const CHAR *Format,
	...
);