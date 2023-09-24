#include "String.h"
#include "Memory.h"

#define PRINT_GRANULARITY (32 * sizeof(CHAR16))

INTN
StrLen(
	IN const CHAR16 *String
) {
	INTN Length = 0;

	for (; *String != 0; Length++, String++);

	return Length;
}

INTN
StrLen8(
	IN const CHAR8 *String
) {
	INTN Length = 0;

	for (; *String != 0; Length++, String++);
	
	return Length;
}

BOOLEAN
WriteHexadecimal(
	IN  UINT64  Integer,
	OUT INTN   *Length,
	OUT CHAR16 *String,
	IN  INTN    Size
) {
	INTN _Length = 0;

	for (UINT64 Temp = Integer; Temp != 0; _Length++)
		Temp /= 16;
	
	*Length = _Length;

	if (Size < (_Length + 1) * sizeof(CHAR16))
		return FALSE;

	for (INTN i = _Length - 1; i >= 0; i--, Integer /= 16) {
		UINT64 Digit = Integer % 16;
		if (Digit < 10)
			String[i] = L'0' + (CHAR16)Digit;
		else
			String[i] = L'a' + (CHAR16)(Digit - 10);
	}

	return TRUE;
}

BOOLEAN
WriteUnsignedDecimal(
	IN  UINT64  Integer,
	OUT INTN   *Length,
	OUT CHAR16 *String,
	IN  INTN    Size
) {
	INTN _Length = 0;

	for (UINT64 Temp = Integer; Temp != 0; _Length++)
		Temp /= 10;
	
	*Length = _Length;
	
	if (Size < (_Length + 1) * sizeof(CHAR16))
		return FALSE;

	for (INTN i = _Length - 1; i >= 0; i--, Integer /= 10)
		String[i] = L'0' + (CHAR16)(Integer % 10);

	return TRUE;
}

BOOLEAN
WriteSignedDecimal(
	IN  INT64   Integer,
	OUT INTN   *Length,
	OUT CHAR16 *String,
	IN  INTN    Size
) {
	INTN Offset = Integer < 0 ? 1 : 0;
	INTN _Length = Offset;

	for (INT64 Temp = Integer; Temp != 0; _Length++) 
		Temp /= 10;

	*Length = _Length;
	
	if (Size < (_Length + 1) * sizeof(CHAR16))
		return FALSE;

	if (Integer < 0) {
		String[0] = L'-';
		Integer = -Integer;
	}

	for (INTN i = _Length - 1; i >= Offset; i--, Integer /= 10)
		String[i] = L'0' + (CHAR16)(Integer % 10);

	return TRUE;
}

VOID
PrintVariadic(
	IN const CHAR16 *Format,
	IN VA_LIST       Args
) {
#define RESIZE(NewSize) { \
	ReallocatePool((VOID**)&String, NewSize, Size); \
	Size = NewSize; \
}
	INTN    Size            = EXTEND((StrLen(Format) + 1) * sizeof(CHAR16), PRINT_GRANULARITY);
	CHAR16 *String          = AllocatePool(Size);
	BOOLEAN FormatSpecifier = FALSE;

	for (INTN i = 0; *Format != 0; i++, Format++) {
		if ((i + 1) * sizeof(CHAR16) >= Size)
			RESIZE(Size + PRINT_GRANULARITY);

		if (*Format == L'%') {
			if (FormatSpecifier)
				String[i] = L'%';
			else
				i--;

			FormatSpecifier ^= TRUE;
		} else if (FormatSpecifier) {
			switch (*Format) {
			case L's': {
				const CHAR16 *StringArg = VA_ARG(Args, const CHAR16 *);
				INTN          Length    = StrLen(StringArg);

				while ((i + Length + 1) * sizeof(CHAR16) >= Size)
					RESIZE(EXTEND(Size + Length * sizeof(CHAR16), PRINT_GRANULARITY));
				
				CopyMemory(String + i, (VOID*)StringArg, (Length + 1) * sizeof(CHAR16));
				i += Length - 1;

				break;
			}
			case L'S': {
				const CHAR8 *StringArg = VA_ARG(Args, const CHAR8 *);
				INTN         Length    = StrLen8(StringArg);

				while ((i + Length + 1) * sizeof(CHAR16) >= Size)
					RESIZE(EXTEND(Size + Length * sizeof(CHAR16), PRINT_GRANULARITY));
				
				for (INTN j = 0; j < Length; j++)
					String[i + j] = (CHAR16)StringArg[j];
				
				i += Length - 1;

				break;
			}
			case L'u': {
				UINT64 Integer = VA_ARG(Args, UINT64);
				INTN   Length  = 0;

				while (!WriteUnsignedDecimal(Integer, &Length, String + i, Size - i * sizeof(CHAR16)))
					RESIZE(EXTEND(Size + Length * sizeof(CHAR16), PRINT_GRANULARITY));
				
				i += Length - 1;

				break;
			}
			case L'd': {
				INT64 Integer = VA_ARG(Args, INT64);
				INTN  Length  = 0;

				while (!WriteSignedDecimal(Integer, &Length, String + i, Size - i * sizeof(CHAR16)))
					RESIZE(EXTEND(Size + Length * sizeof(CHAR16), PRINT_GRANULARITY));
				
				i += Length - 1;

				break;
			}
			case L'x': {
				UINT64 Integer = VA_ARG(Args, UINT64);
				INTN   Length  = 0;

				while (!WriteHexadecimal(Integer, &Length, String + i, Size - i * sizeof(CHAR16)))
					RESIZE(EXTEND(Size + Length * sizeof(CHAR16), PRINT_GRANULARITY));
				
				i += Length - 1;

				break;
			}
			}

			FormatSpecifier = FALSE;
		} else {
			String[i] = *Format;
		}
	}

	ST->ConOut->OutputString(ST->ConOut, String);

	FreePool(String);
#undef RESIZE
}

VOID
Print(
	IN const CHAR16 *Format,
	...
) {
	VA_LIST Args;
	VA_START(Args, Format);

	PrintVariadic(Format, Args);

	VA_END(Args);
}