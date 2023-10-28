#pragma once

#include <Phosbase.h>

#define _PHOSKRNL_HAL_H_

#define COM1 0x3F8

#define UART_IER 1
#define UART_FCR 2
#define UART_LCR 3
#define UART_MCR 4
#define UART_LSR 5
#define UART_MSR 6
#define UART_SCR 7

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

EXPORT
VOID
PHOSAPI
HalCreateGdtr(
	IN OUT HALGDTR *Reg
);

__forceinline
EXPORT
VOID
PHOSAPI
HalLoadGdtr(
	IN HALGDTR *Reg
) {
	__asm lgdt [Reg];
}

BOOL
PHOSAPI
HalInitSerial(
	IN UINT16 Port 
);

EXPORT
UINT8
PHOSAPI
HalPollSerial(
	IN UINT16 Port 
);

EXPORT
UINT8
PHOSAPI
HalReadSerial(
	IN UINT16 Port 
);

EXPORT
VOID
PHOSAPI
HalWriteSerial(
	IN UINT16 Port,
	IN UINT8  Value
);

EXPORT
VOID
PHOSAPI
HalWriteSerialEx(
	IN UINT16       Port,
	IN const UINT8 *Data,
	IN UINTN        Size
);