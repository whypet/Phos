#pragma once

#include "Phosboot.h"

#ifdef DEBUG
#define ASSERT(Cond) \
	if (!(Cond)) AssertFail(#Cond, __FILE__, __LINE__)
#else
#define ASSERT(Cond)
#endif

VOID
AssertFail(
	IN CONST CHAR8 *Cond,
	IN CONST CHAR8 *File,
	IN INT32        Line
);