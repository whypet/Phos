#pragma once

#include <PhosBase.h>

#pragma pack(push, 1)

typedef struct {
	UINT16 Limit;
	UINT64 Base;
} HALGDTR;

#pragma pack(pop)

// fields prefixed with an underscore are unused under long mode
// _LimitX fields make up a 20-bit value and _BaseX fields make up a 32-bit value (applies to protected mode)
typedef struct {
	UINT64 _LimitA : 16;
	UINT64 _BaseA  : 16;
	UINT64 _BaseB  : 8;

	struct {
		UINT8 Accessed            : 1;
		UINT8 ReadWrite           : 1;
		UINT8 DirectionConforming : 1;
		UINT8 Executable          : 1;
		UINT8 Type                : 1;
		UINT8 Dpl                 : 2;
		UINT8 Present             : 1;
	} Access;
	
	UINT64 _LimitB : 4;

	UINT64 Reserved        : 1;
	UINT64 CodeFlag        : 1;
	UINT64 SizeFlag        : 1;
	UINT64 GranularityFlag : 1;

	UINT64 _BaseC : 8;
} HALGDT_SEGMENT_DESCRIPTOR;

VOID
PHOSAPI
HalCreateGdtr(
	IN OUT HALGDTR *Reg
);

__forceinline
VOID
PHOSAPI
HalLoadGdtr(
	IN HALGDTR *Reg
) {
	__asm lgdt [Reg];
}