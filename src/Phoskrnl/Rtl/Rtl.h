#pragma once

#include <Phosbase.h>

#define _PHOSKRNL_RTL_H_

#define strlen        RtlStringSizeA
#define RtlStringLenA RtlStringSizeA

inline
EXPORT
VOID
PHOSAPI
RtlZeroMemory(
	IN OUT VOID *Dst,
	IN     UINTN Size
);

inline
EXPORT
VOID
PHOSAPI
RtlSetMemory(
	IN OUT VOID *Dst,
	IN     UINT8 Val,
	IN     UINTN Size
);

inline
EXPORT
VOID
PHOSAPI
RtlCopyMemory(
	IN OUT VOID       *Dst,
	IN     const VOID *Src,
	IN     UINTN       Size
);

inline
EXPORT
UINTN
PHOSAPI
RtlStringSizeA(
	IN const CHAR8 *Data
);