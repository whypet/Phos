#include "PhosBoot.h"
#include "String.h"
#include "Debug.h"

#define WaitForEvent(Event) { \
	UINTN Index = 0; \
	BS->WaitForEvent(1, Event, &Index); \
}

EFI_SYSTEM_TABLE     *ST;
EFI_BOOT_SERVICES    *BS;
EFI_RUNTIME_SERVICES *RS;

EFI_STATUS
EFIAPI
EfiMain(
	IN EFI_HANDLE        ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable
) {	
	EFI_STATUS Status = EFI_SUCCESS;

	ST = SystemTable;
	BS = SystemTable->BootServices;
	RS = SystemTable->RuntimeServices;

    EFI_LOADED_IMAGE *LoadedImage = NULL;

	if (EFI_ERROR(Status = BS->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (VOID**)&LoadedImage))) {
		goto Cleanup;
	}

#if DEBUG
	Print(L"image base: 0x%x; waiting for debugger.\r\n\r\n", LoadedImage->ImageBase);
	{
		volatile BOOLEAN Pause = TRUE;
		while (Pause)
			__asm pause;
	}
#endif

	Print(L"unsigned decimal test: %u\r\n", 0xffffffffffffffff);

	WaitForEvent(&ST->ConIn->WaitForKey);

Cleanup:
	return Status;
}