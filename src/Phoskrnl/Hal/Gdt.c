#include "Hal.h"

#include <Rtl/Rtl.h>

VOID
PHOSAPI
HalGdtrInsert(
	IN OUT KGDTR64    *Gdtr,
	IN     CONST VOID *Segment,
	IN     UINTN       SegmentSize
) {
	UINT8 *Pointer;

	Pointer = (UINT8 *)Gdtr->Base + Gdtr->Limit + 1;

	RtlCopyMemory(Pointer, Segment, SegmentSize);

	Gdtr->Limit += SegmentSize;
}

VOID
PHOSAPI
HalFillGdt(
	IN OUT KGDTR64 *Gdtr,
	IN     UINTN    GdtSize
) {
	KGDT_SEGMENT64        Segment = { 0 };
	KGDT_SYSTEM_SEGMENT64 Tss     = { 0 };
	UINT64                Zero    = 0;

	ASSERT(GdtSize >= 1 * sizeof(UINT64) + 2 * sizeof(KGDT_SEGMENT64) + 1 * sizeof(KGDT_SYSTEM_SEGMENT64));

	// Null descriptor

	HalGdtrInsert(Gdtr, &Zero, sizeof(Zero));

	// Kernel mode code segment

	Segment.Access.CodeType.Accessed   = 1;
	Segment.Access.CodeType.Readable   = 1;
	Segment.Access.CodeType.Conforming = 0; // Driver code on ring 1
	Segment.Access.CodeType.Executable = 1;

	Segment.Access.Type      = 1;
	Segment.Access.Privilege = 0; // Ring 0
	Segment.Access.Present   = 1;

	Segment.Flags.Free          = 0;
	Segment.Flags.Code64Bit     = 1;
	Segment.Flags.OperationSize = 0;
	Segment.Flags.Granularity   = 1;

	HalGdtrInsert(Gdtr, &Segment, sizeof(Segment));

	// Kernel mode data segment

	Segment.Access.DataType.Accessed   = 1;
	Segment.Access.DataType.Writable   = 1;
	Segment.Access.DataType.Direction  = 1;
	Segment.Access.DataType.Executable = 0;

	Segment.Access.Type      = 1;
	Segment.Access.Privilege = 0;
	Segment.Access.Present   = 1;

	Segment.Flags.Free          = 0;
	Segment.Flags.Code64Bit     = 0;
	Segment.Flags.OperationSize = 1;
	Segment.Flags.Granularity   = 1;

	HalGdtrInsert(Gdtr, &Segment, sizeof(Segment));

	// Task state segment

	// Will do when SMP/scheduling is implemented

	HalGdtrInsert(Gdtr, &Tss, sizeof(Tss));
}

VOID
PHOSAPI
HalCreateGdtr(
	IN OUT KGDTR64    *Reg,
	IN     CONST VOID *Gdt,
	IN     UINT64      GdtSize
) {
	Reg->Limit = GdtSize - 1;
	Reg->Base  = (UINT64)Gdt;
}