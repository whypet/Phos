#include <Phos.h>

VOID
PHOSAPI
KiMain(
	IN VIDEO_INFO Video
) {
	HalInitSerial(COM1);

	HalWriteSerial(COM1, 'a');
	HalWriteSerial(COM1, 'b');
	HalWriteSerial(COM1, 'c');

	for (;;) {
		__asm__ __volatile__("pause");
	}

	return;
}