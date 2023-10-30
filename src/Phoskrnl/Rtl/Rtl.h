#pragma once

#include <Phosbase.h>

#define _PHOSKRNL_RTL_H_

#define strlen        RtlStringSizeA
#define RtlStringLenA RtlStringSizeA

#pragma region Memory

INLINE
PHOSKRNLAPI
VOID
PHOSAPI
RtlZeroMemory(
	IN OUT VOID *Dst,
	IN     UINTN Size
);

INLINE
PHOSKRNLAPI
VOID
PHOSAPI
RtlSetMemory(
	IN OUT VOID *Dst,
	IN     UINT8 Val,
	IN     UINTN Size
);

INLINE
PHOSKRNLAPI
VOID
PHOSAPI
RtlCopyMemory(
	IN OUT VOID       *Dst,
	IN     CONST VOID *Src,
	IN     UINTN       Size
);

#pragma endregion

#pragma region Strings

INLINE
PHOSKRNLAPI
UINTN
PHOSAPI
RtlStringSizeA(
	IN CONST CHAR8 *Data
);

#pragma endregion