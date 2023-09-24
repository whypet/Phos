#pragma once

#include "PhosBoot.h"

#ifdef DEBUG
#define ASSERT(cond) { \
	if (!(cond)) \
		AssertFail(#cond, __FILE__, __LINE__); \
}
#else
#define ASSERT(cond)
#endif

VOID
AssertFail(
	const CHAR8 *Cond,
	const CHAR8 *File,
	INT32        Line
);