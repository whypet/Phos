#include "String.h"
#include "Memory.h"

#define ALIGNMENT (32 * sizeof(CHAR16))

UINTN
StrLen(
	IN CONST CHAR16 *String
) {
	UINTN Length = 0;

	for (; *String != 0; Length++, String++);

	return Length;
}

UINTN
StrLen8(
	IN CONST CHAR8 *String
) {
	UINTN Length = 0;

	for (; *String != 0; Length++, String++);

	return Length;
}

BOOLEAN
WriteHexadecimal(
	IN     UINT64  Integer,
	IN OUT INTN   *Length,
	IN OUT CHAR16 *String,
	IN     UINTN   Size
) {
	INTN _Length = 0;

	if (Integer == 0)
		_Length = 1;
	else {
		for (UINT64 Temp = Integer; Temp != 0; _Length++)
			Temp /= 16;
	}

	*Length = _Length;

	if (Size < (UINTN)((_Length + 1) * sizeof(CHAR16)))
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
	IN     UINT64  Integer,
	IN OUT INTN   *Length,
	IN OUT CHAR16 *String,
	IN     UINTN   Size
) {
	INTN _Length = 0;

	if (Integer == 0)
		_Length = 1;
	else {
		for (UINT64 Temp = Integer; Temp != 0; _Length++)
			Temp /= 10;
	}
	
	*Length = _Length;
	
	if (Size < (_Length + 1) * sizeof(CHAR16))
		return FALSE;

	for (INTN i = _Length - 1; i >= 0; i--, Integer /= 10)
		String[i] = L'0' + (CHAR16)(Integer % 10);

	return TRUE;
}

BOOLEAN
WriteSignedDecimal(
	IN     INT64   Integer,
	IN OUT INTN   *Length,
	IN OUT CHAR16 *String,
	IN     UINTN   Size
) {
	INTN Offset = Integer < 0 ? 1 : 0;
	INTN _Length = Offset;

	if (Integer == 0)
		_Length = 1;
	else {
		for (INT64 Temp = Integer; Temp != 0; _Length++) 
			Temp /= 10;
	}

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
	IN CONST CHAR16 *Format,
	IN VA_LIST       Args
) {
#define RESIZE(NewSize) { \
	ReallocatePool((VOID**)&String, NewSize, Size); \
	Size = NewSize; \
}
	INTN    Size            = ALIGN((StrLen(Format) + 1) * sizeof(CHAR16), ALIGNMENT);
	CHAR16 *String          = AllocatePool(Size);
	BOOLEAN FormatSpecifier = FALSE;

	for (INTN i = 0; *Format != 0; i++, Format++) {
		if ((i + 1) * sizeof(CHAR16) >= Size)
			RESIZE(Size + ALIGNMENT);

		if (*Format == L'%') {
			if (FormatSpecifier)
				String[i] = L'%';
			else
				i--;

			FormatSpecifier ^= TRUE;
		} else if (FormatSpecifier) {
			switch (*Format) {
			case L'c': {
				CHAR16 Char = (CHAR16)VA_ARG(Args, INT32);
				
				String[i] = Char;

				break;
			}
			case L'C': {
				CHAR8 Char = (CHAR16)VA_ARG(Args, INT32);
				
				String[i] = (CHAR16)Char; // zero-extend (bmp is the same for both utf-16 and ansi)

				break;
			}
			case L's': {
				CHAR16 *StringArg = VA_ARG(Args, CHAR16 *);
				UINTN   Length    = StrLen(StringArg);

				while ((i + Length + 1) * sizeof(CHAR16) >= Size)
					RESIZE(ALIGN(Size + Length * sizeof(CHAR16), ALIGNMENT));
				
				CopyMemory(String + i, (VOID*)StringArg, (Length + 1) * sizeof(CHAR16));
				i += Length - 1;

				break;
			}
			case L'S': {
				CHAR8 *StringArg = VA_ARG(Args, CHAR8 *);
				UINTN  Length    = StrLen8(StringArg);

				while ((i + Length + 1) * sizeof(CHAR16) >= Size)
					RESIZE(ALIGN(Size + Length * sizeof(CHAR16), ALIGNMENT));
				
				for (INTN j = 0; j < Length; j++)
					String[i + j] = (CHAR16)StringArg[j];
				
				i += Length - 1;

				break;
			}
			case L'u': {
				UINT64 Integer = VA_ARG(Args, UINT64);
				INTN   Length  = 0;

				while (!WriteUnsignedDecimal(Integer, &Length, String + i, Size - i * sizeof(CHAR16)))
					RESIZE(ALIGN(Size + Length * sizeof(CHAR16), ALIGNMENT));
				
				i += Length - 1;

				break;
			}
			case L'd': {
				INT64 Integer = VA_ARG(Args, INT64);
				INTN  Length  = 0;

				while (!WriteSignedDecimal(Integer, &Length, String + i, Size - i * sizeof(CHAR16)))
					RESIZE(ALIGN(Size + Length * sizeof(CHAR16), ALIGNMENT));
				
				i += Length - 1;

				break;
			}
			case L'x': {
				UINT64 Integer = VA_ARG(Args, UINT64);
				INTN   Length  = 0;

				while (!WriteHexadecimal(Integer, &Length, String + i, Size - i * sizeof(CHAR16)))
					RESIZE(ALIGN(Size + Length * sizeof(CHAR16), ALIGNMENT));
				
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
	IN CONST CHAR16 *Format,
	...
) {
	VA_LIST Args;
	VA_START(Args, Format);

	PrintVariadic(Format, Args);

	VA_END(Args);
}