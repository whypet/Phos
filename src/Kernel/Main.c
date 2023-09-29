#include "Phos.h"

VOID
PHOSAPI
KiMain(
	VOID // add rsdp etc later
) {
	for (;;)
		__asm pause;

	return;
}