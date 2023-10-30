#pragma once

#include "Phosboot.h"

VOID
ZeroMemory(
	IN OUT VOID *Dst,
	IN     UINTN Size
);

VOID
CopyMemory(
	IN OUT VOID       *Dst,
	IN     CONST VOID *Src,
	IN     UINTN       Size
);

VOID *
AllocatePool(
	IN UINTN Size
);

VOID
ReallocatePool(
	IN OUT VOID **Buffer,
	IN     UINTN  NewSize,
	IN     UINTN  OldSize
);

FORCEINLINE
VOID
FreePool(
	IN OUT VOID *Buffer
) {
	BS->FreePool(Buffer);
}