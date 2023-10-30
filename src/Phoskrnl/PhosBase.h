#pragma once

#define _PHOS_ 1

#include <Phosdef.h>
#include <Intrinsics.h>

#include <assert.h> // for static_assert
#undef assert

#define ASSERT(Cond) \
	if (!(Cond)) KiAssertFail(#Cond, __FILE__, __LINE__)
#define C_ASSERT(Cond) static_assert(Cond, #Cond)

typedef _Bool BOOLEAN;

VOID
PHOSAPI
KiAssertFail(
	IN CONST CHAR8 *Cond,
	IN CONST CHAR8 *File,
	IN INT32        Line
);