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
	IN const CHAR8 *Cond,
	IN const CHAR8 *File,
	IN INT32        Line
);