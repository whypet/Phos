#pragma once

#include "Phosboot.h"

VOID
ZeroMemory(
	IN OUT VOID *Buffer,
	IN     INTN  Size
);

VOID
CopyMemory(
	IN OUT VOID *Dst,
	IN     VOID *Src,
	IN     INTN  Size
);

VOID*
AllocatePool(
	IN UINTN Size
);

VOID
ReallocatePool(
	IN OUT VOID **Buffer,
	IN     UINTN  NewSize,
	IN     UINTN  OldSize
);

__forceinline
VOID
FreePool(
	IN OUT VOID *Buffer
) {
	BS->FreePool(Buffer);
}