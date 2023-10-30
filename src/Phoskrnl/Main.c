#include <Phos.h>

VOID
PHOSAPI
KiMain(
	IN KINIT_VID_INFO *Video
) {
	KiSystemInit();

	KdPrint("Hello world from Phoskrnl");

	for (;;)
		__asm__ __volatile__("pause");

	return;
}