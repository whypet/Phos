#include "Ke.h"

#include <Hal/Hal.h>
#include <Kd/Kd.h>

VOID
PHOSAPI
KiSystemInit(
	VOID
) {
	HalInitSerial(COM1);

	KdPrint("Starting system...");
	KdPrint("[*] Setting up GDT...");

	{
		UINT8   Gdt[1 * sizeof(UINT64) + 2 * sizeof(KGDT_SEGMENT64) + 1 * sizeof(KGDT_SYSTEM_SEGMENT64)];
		KGDTR64 Gdtr = {
			.Base = (UINT64)Gdt,
			.Limit = (UINT16)-1
		};

		HalFillGdt(&Gdtr, sizeof(Gdt));
		HalLoadGdtr((CONST KGDTR64 *)&Gdtr);
	}

	KdPrint("Finished loading.");
	KdPrint("");
}

VOID
PHOSAPI
KiHalt(
	VOID
) {
	for (;;)
		__asm__ __volatile__("hlt");
}

VOID
PHOSAPI
KiAssertFail(
	IN CONST CHAR8 *Cond,
	IN CONST CHAR8 *File,
	IN INT32        Line
) {
	// Replace this later when Printf is implemented
	KdPrint("Assertion failure!");
	KdPrint(Cond);
	KdPrint("");

	KePanic("Assertion failure");
}

VOID
PHOSAPI
KePanic(
	CONST CHAR8 *Reason
) {
	// Replace this later when Printf is implemented
	KdPrint("********** KERNEL PANIC **********");
	KdPrint("");
	KdPrint("Reason:");
	KdPrint(Reason);
	KdPrint("");
	KdPrint("Halting system.");
	KdPrint("");
	KdPrint("**********************************");

	KiHalt();
}