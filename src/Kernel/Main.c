#include "Phos.h"

VOID
PHOSAPI
KiMain(
	IN VIDEO_MODE VidMode
) {
	for (;;)
		__asm pause;

	return;
}