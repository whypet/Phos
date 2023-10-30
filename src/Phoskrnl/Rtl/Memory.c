#include "Rtl.h"

//
// These functions will be optimized later to take alignment into account and with SIMD instructions.
// For now, they act like a regular movsb.
//

INLINE
VOID
RtlZeroMemory(
	IN OUT VOID *Dst,
	IN     UINTN Size
) {
	for (UINTN i = 0; i < Size; i++)
		*((INT8 *)Dst + i) = 0;
}

INLINE
VOID
RtlSetMemory(
	IN OUT VOID *Dst,
	IN     UINT8 Val,
	IN     UINTN Size
) {
	for (UINTN i = 0; i < Size; i++)
		*((INT8 *)Dst + i) = Val;
}

INLINE
VOID
RtlCopyMemory(
	IN OUT VOID       *Dst,
	IN     CONST VOID *Src,
	IN     UINTN       Size
) {
	for (UINTN i = 0; i < Size; i++)
		*((INT8 *)Dst + i) = *((INT8 *)Src + i);
}