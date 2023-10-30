#include "Rtl.h"

INLINE
UINTN
RtlStringSizeA(
	IN CONST CHAR8 *Data
) {
	UINTN Size;

	for (Size = 0; *Data != '\0'; Size++, Data++);

	return Size;
}