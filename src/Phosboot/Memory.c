#include "Memory.h"
#include "Debug.h"

VOID
ZeroMemory(
	IN OUT VOID *Dst,
	IN     UINTN Size
) {
	for (UINTN i = 0; i < Size; i++)
		*((INT8*)Dst + i) = 0;
}

VOID
CopyMemory(
	IN OUT VOID       *Dst,
	IN     CONST VOID *Src,
	IN     UINTN       Size
) {
	for (UINTN i = 0; i < Size; i++)
		*((INT8*)Dst + i) = *((INT8*)Src + i);
}

VOID *
AllocatePool(
	IN UINTN Size
) {
	VOID *Buffer;

	BS->AllocatePool(EfiBootServicesData, Size, &Buffer);
	ZeroMemory(Buffer, Size);

	ASSERT(Buffer != NULL);

	return Buffer;
}

VOID
ReallocatePool(
	IN OUT VOID **Buffer,
	IN     UINTN  NewSize,
	IN     UINTN  OldSize
) {
	VOID *NewBuffer = AllocatePool(NewSize);

	CopyMemory(NewBuffer, *Buffer, OldSize);
	ZeroMemory(NewBuffer + OldSize, NewSize - OldSize);

	FreePool(*Buffer);

	*Buffer = NewBuffer;
}