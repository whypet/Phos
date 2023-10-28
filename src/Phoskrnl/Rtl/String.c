#include "Rtl.h"

inline
UINTN
RtlStringSizeA(
	IN const CHAR8 *Data
) {
	UINTN Size;

	for (Size = 0; *Data != '\0'; Size++, Data++);

	return Size;
}