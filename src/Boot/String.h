#pragma once

#include "PhosBoot.h"

INTN
StrLen(
	IN const CHAR16 *String
);

INTN
StrLen8(
	IN const CHAR8 *String
);

BOOLEAN
WriteUnsignedDecimal(
	IN  UINT64  Decimal,
	OUT INTN   *Length,
	OUT CHAR16 *String,
	IN  INTN    Size
);

BOOLEAN
WriteSignedDecimal(
	IN  INT64   Decimal,
	OUT INTN   *Length,
	OUT CHAR16 *String,
	IN  INTN    Size
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