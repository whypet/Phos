#pragma once

#include "Phosboot.h"

INTN
StrLen(
	IN const CHAR16 *String
);

INTN
StrLen8(
	IN const CHAR8 *String
);

BOOLEAN
WriteHexadecimal(
	IN     UINT64  Integer,
	IN OUT INTN   *Length,
	IN OUT CHAR16 *String,
	IN     INTN    Size
);

BOOLEAN
WriteUnsignedDecimal(
	IN     UINT64  Decimal,
	IN OUT INTN   *Length,
	IN OUT CHAR16 *String,
	IN     INTN    Size
);

BOOLEAN
WriteSignedDecimal(
	IN     INT64   Decimal,
	IN OUT INTN   *Length,
	IN OUT CHAR16 *String,
	IN     INTN    Size
);

VOID
PrintVariadic(
	IN const CHAR16 *Format,
	IN VA_LIST       Args
);

VOID
Print(
	IN const CHAR16 *Format,
	...
);