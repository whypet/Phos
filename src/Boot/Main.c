#include "PhosBoot.h"
#include "String.h"

#define WaitForEvent(Event) { \
	UINTN Index = 0; \
	BS->WaitForEvent(1, Event, &Index); \
}

EFI_SYSTEM_TABLE     *ST;
EFI_BOOT_SERVICES    *BS;
EFI_RUNTIME_SERVICES *RS;

EFI_STATUS
GetVolume(
	IN  EFI_LOADED_IMAGE *LoadedImage,
	OUT EFI_FILE_HANDLE  *Volume
) {
	EFI_STATUS Status;

	EFI_FILE_IO_INTERFACE *Io = NULL;

	if (EFI_ERROR(Status = BS->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (VOID **)&Io)))
		return Status;

	Io->OpenVolume(Io, Volume);

	return Status;
}

EFI_STATUS
EFIAPI
EfiMain(
	IN EFI_HANDLE        ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable
) {	
	EFI_STATUS Status;

	ST = SystemTable;
	BS = SystemTable->BootServices;
	RS = SystemTable->RuntimeServices;

	EFI_LOADED_IMAGE *LoadedImage = NULL;

	if (EFI_ERROR(Status = BS->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (VOID**)&LoadedImage))) {
		Print(L"failed to get the loaded efi image.\r\n");
		goto Cleanup;
	}

#if DEBUG
	{
		Print(L"image base: 0x%x; waiting for debugger.\r\n\r\n", LoadedImage->ImageBase);

		volatile BOOLEAN Pause = TRUE;
		while (Pause)
			__asm pause;
	}
#endif

	EFI_FILE_HANDLE Volume = NULL;

	if (EFI_ERROR(Status = GetVolume(LoadedImage, &Volume))) {
		Print(L"failed to open the volume in which this image is located.\r\n");
		goto Cleanup;
	}

	EFI_FILE_HANDLE FileHandle = NULL;

	if (EFI_ERROR(Status = Volume->Open(Volume, &FileHandle, KERNEL_FILENAME, EFI_FILE_MODE_READ, 0))) {
		Print(L"failed to open kernel file handle.\r\n");
		goto Cleanup;
	}

	WaitForEvent(&ST->ConIn->WaitForKey);
Cleanup:
	if (EFI_ERROR(Status))
		Print(L"status: %x\r\n", Status);

	return Status;
}