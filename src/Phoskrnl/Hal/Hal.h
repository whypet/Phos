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

#define SYSTEM_SEGMENT_TSS_AVAILABLE64 9

#pragma pack(push, 1)

typedef struct {
	UINT16 Limit;
	UINT64 Base;
} KGDTR64;

C_ASSERT(sizeof(KGDTR64) == 10);

// Limit and Base fields are unused under long mode
typedef struct {
	UINT16 LimitPart1;
	UINT16 BasePart1;
	UINT8  BasePart2;

	struct {
		union {
			struct {
				UINT8 Accessed   : 1;
				UINT8 Writable   : 1;
				UINT8 Direction  : 1;
				UINT8 Executable : 1; // Set to 0
				UINT8 _          : 4;
			} DataType;
			struct {
				UINT8 Accessed   : 1;
				UINT8 Readable   : 1;
				UINT8 Conforming : 1;
				UINT8 Executable : 1; // Set to 1
				UINT8 _          : 4;
			} CodeType;
			struct {
				UINT8 _         : 4;
				UINT8 Type      : 1; // Set to 1
				UINT8 Privilege : 2;
				UINT8 Present   : 1;
			};
		};
	} Access;

	union {
		UINT8 LimitPart2 : 4;
		struct {
			UINT8 _             : 4;
			UINT8 Free          : 1;
			UINT8 Code64Bit     : 1;
			UINT8 OperationSize : 1;
			UINT8 Granularity   : 1;
		} Flags;
	};

	UINT8 BasePart3;
} KGDT_SEGMENT64;

C_ASSERT(sizeof(KGDT_SEGMENT64) == 8);

typedef struct {
	UINT16 LimitPart1;
	UINT16 BasePart1;
	UINT8  BasePart2;

	struct {
		UINT8 Type      : 4;
		UINT8 Reserved  : 1;
		UINT8 Privilege : 2;
		UINT8 Present   : 1;
	} Access;

	union {
		UINT8 LimitPart2 : 4;
		struct {
			UINT8 _           : 4;
			UINT8 Reserved    : 3;
			UINT8 Granularity : 1;
		} Flags;
	};

	UINT32 BasePart3;
	UINT8  BasePart4;
	UINT32 Reserved;
} KGDT_SYSTEM_SEGMENT64;

C_ASSERT(sizeof(KGDT_SYSTEM_SEGMENT64) == 16);

typedef struct {
	UINT32 Reserved1;
	UINT64 Rsp[3];
	UINT32 Reserved2[2];
	UINT64 Ist[7];
	UINT32 Reserved3[2];
	UINT16 Reserved4;
	UINT16 IoMapBase;
} KTASK_STATE64;

C_ASSERT(sizeof(KTASK_STATE64) == 104);

#pragma pack(pop)

#pragma region GDT

PHOSKRNLAPI
VOID
PHOSAPI
HalGdtrInsert(
	IN OUT KGDTR64    *Gdtr,
	IN     CONST VOID *Segment,
	IN     UINTN       SegmentSize
);

PHOSKRNLAPI
VOID
PHOSAPI
HalFillGdt(
	IN OUT KGDTR64 *Gdtr,
	IN     UINTN    GdtSize
);

PHOSKRNLAPI
VOID
PHOSAPI
HalCreateGdtr(
	IN OUT KGDTR64    *Reg,
	IN     CONST VOID *Gdt,
	IN     UINT64      GdtSize
);

PHOSKRNLAPI
VOID
PHOSAPI
HalLoadGdtr(
	IN CONST KGDTR64 *Reg
);

#pragma endregion

#pragma region UART

PHOSKRNLAPI
BOOLEAN
PHOSAPI
HalInitSerial(
	IN UINT16 Port 
);

PHOSKRNLAPI
UINT8
PHOSAPI
HalPollSerial(
	IN UINT16 Port 
);

PHOSKRNLAPI
UINT8
PHOSAPI
HalReadSerial(
	IN UINT16 Port 
);

PHOSKRNLAPI
VOID
PHOSAPI
HalWriteSerial(
	IN UINT16 Port,
	IN UINT8  Value
);

PHOSKRNLAPI
VOID
PHOSAPI
HalWriteSerialEx(
	IN UINT16       Port,
	IN CONST UINT8 *Data,
	IN UINTN        Size
);

#pragma endregion