#include <Phos.h>

VOID
PHOSAPI
KiMain(
	IN VIDEO_INFO Video
) {
	for (;;)
		__asm pause;

	return;
}