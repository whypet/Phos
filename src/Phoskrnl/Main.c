#include <Phos.h>

VOID
PHOSAPI
KiMain(
	IN VIDEO_INFO Video
) {
	HalInitSerial(COM1);

	for (;;)
		__asm__ __volatile__("pause");

	return;
}