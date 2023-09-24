#pragma once

#include "PhosBoot.h"

VOID
ZeroMemory(
	IN VOID *Buffer,
	IN INTN  Size
);

VOID
CopyMemory(
	IN VOID *Dst,
	IN VOID *Src,
	IN INTN  Size
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

VOID
__forceinline
FreePool(
	IN VOID *Buffer
) {
	BS->FreePool(Buffer);
}