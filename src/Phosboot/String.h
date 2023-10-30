#pragma once

#include "Phosboot.h"

UINTN
StrLen(
	IN CONST CHAR16 *String
);

UINTN
StrLen8(
	IN CONST CHAR8 *String
);

BOOLEAN
WriteHexadecimal(
	IN     UINT64  Integer,
	IN OUT INTN   *Length,
	IN OUT CHAR16 *String,
	IN     UINTN   Size
);

BOOLEAN
WriteUnsignedDecimal(
	IN     UINT64  Decimal,
	IN OUT INTN   *Length,
	IN OUT CHAR16 *String,
	IN     UINTN   Size
);

BOOLEAN
WriteSignedDecimal(
	IN     INT64   Decimal,
	IN OUT INTN   *Length,
	IN OUT CHAR16 *String,
	IN     UINTN   Size
);

VOID
PrintVariadic(
	IN CONST CHAR16 *Format,
	IN VA_LIST       Args
);

VOID
Print(
	IN CONST CHAR16 *Format,
	...
);