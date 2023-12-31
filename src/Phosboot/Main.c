#include "Phosboot.h"
#include "Debug.h"
#include "Loader.h"
#include "Memory.h"
#include "String.h"

#define WAIT_FOR_EVENT(Event)               \
	do {                                    \
		UINTN Index = 0;                    \
		BS->WaitForEvent(1, Event, &Index); \
	} while (0)

#define PRINT_ERROR(Message, ...)                                                \
	do {                                                                         \
		ST->ConOut->SetAttribute(ST->ConOut, EFI_TEXT_ATTR(EFI_WHITE, EFI_RED)); \
		Print(Message, ##__VA_ARGS__);                                           \
		ST->ConOut->SetAttribute(ST->ConOut, EFI_RED | EFI_BRIGHT);              \
	} while (0)

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
OpenFile(
	IN  EFI_LOADED_IMAGE *LoadedImage,
	IN  CONST CHAR16     *Filename,
	OUT EFI_FILE_HANDLE  *FileHandle
) {
	EFI_STATUS Status;

	EFI_FILE_HANDLE Volume = NULL;

	if (EFI_ERROR(Status = GetVolume(LoadedImage, &Volume)))
		return Status;

	if (EFI_ERROR(Status = Volume->Open(Volume, FileHandle, (CHAR16 *)Filename, EFI_FILE_MODE_READ, 0)))
		return Status;

	Volume->Close(Volume);

	return Status;
}

EFI_STATUS
GetFileSize(
	IN  EFI_FILE_HANDLE FileHandle,
	OUT UINTN          *FileSize
) {
	EFI_STATUS     Status;
	EFI_FILE_INFO *FileInfo   = NULL;
	UINTN          BufferSize = 0;

	if ((Status = FileHandle->GetInfo(FileHandle, &gEfiFileInfoGuid, &BufferSize, FileInfo)) != EFI_BUFFER_TOO_SMALL)
		return Status;

	FileInfo = (EFI_FILE_INFO *)AllocatePool(BufferSize);

	if (EFI_ERROR(Status = FileHandle->GetInfo(FileHandle, &gEfiFileInfoGuid, &BufferSize, FileInfo)))
		return Status;

	*FileSize = FileInfo->FileSize;

	FreePool((VOID *)FileInfo);

	return Status;
}

EFI_STATUS
ReadFile(
	IN     EFI_FILE_HANDLE FileHandle,
	IN OUT UINTN          *Size,
	OUT    VOID           *Buffer
) {
	EFI_STATUS Status;

	return Status;
}

EFI_STATUS
GetMemoryMap(
	OUT OPTIONAL EFI_MEMORY_DESCRIPTOR **MemoryMap,
	OUT OPTIONAL UINTN                  *MemoryMapSize,
	OUT OPTIONAL UINTN                  *MapKey,
    OUT OPTIONAL UINTN                  *DescriptorSize,
    OUT OPTIONAL UINT32                 *DescriptorVersion
) {
	EFI_STATUS Status;

	EFI_MEMORY_DESCRIPTOR *_MemoryMap         = NULL;
    UINTN                  _MemoryMapSize     = 0;
    UINTN                  _MapKey            = 0;
    UINTN                  _DescriptorSize    = 0;
    UINT32                 _DescriptorVersion = 0;

    Status = BS->GetMemoryMap(
		&_MemoryMapSize,
		_MemoryMap,
		&_MapKey,
		&_DescriptorSize,
		&_DescriptorVersion);

    ASSERT(Status == EFI_BUFFER_TOO_SMALL);

    do {
        _MemoryMapSize += 2 * _DescriptorSize;

        if (_MemoryMap != NULL)
            FreePool(_MemoryMap);
        _MemoryMap = (EFI_MEMORY_DESCRIPTOR *)AllocatePool(_MemoryMapSize);

        ASSERT(_MemoryMap != NULL);

        Status = BS->GetMemoryMap(&_MemoryMapSize, _MemoryMap, &_MapKey, &_DescriptorSize, &_DescriptorVersion);
    } while (Status == EFI_BUFFER_TOO_SMALL);

	if (EFI_ERROR(Status)) {
		FreePool(_MemoryMap);

		return Status;
	}

	if (MemoryMap != NULL)
		*MemoryMap = _MemoryMap;
	if (MemoryMapSize != NULL)
		*MemoryMapSize = _MemoryMapSize;
	if (MapKey != NULL)
		*MapKey = _MapKey;
	if (DescriptorSize != NULL)
		*DescriptorSize = _DescriptorSize;
	if (DescriptorVersion != NULL)
		*DescriptorVersion = _DescriptorVersion;

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

	ST->ConOut->ClearScreen(ST->ConOut);

	ST->ConOut->SetAttribute(ST->ConOut, EFI_TEXT_ATTR(EFI_WHITE, EFI_BLUE));
	ST->ConOut->OutputString(ST->ConOut, L"\r\n  [Phosboot]  \r\n\r\n");
	ST->ConOut->SetAttribute(ST->ConOut, EFI_WHITE);

	EFI_LOADED_IMAGE *LoadedImage = NULL;

	if (EFI_ERROR(Status = BS->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (VOID**)&LoadedImage))) {
		PRINT_ERROR(L"[!] Failed to get the loaded EFI image.\r\n");
		goto Cleanup;
	}
	
#if DEBUGGER
	{
		Print(L"Image base: 0x%x; waiting for debugger.\r\n\r\n", LoadedImage->ImageBase);

		volatile BOOLEAN Pause = TRUE;
		while (Pause)
			__asm pause;
	}
#endif

	EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP = NULL;

	if (EFI_ERROR(Status = BS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID**)&GOP))) {
		PRINT_ERROR(L"[!] Failed to get GOP.\r\n");
		goto Cleanup;
	}

	if (GOP->Mode->Info->PixelFormat != PixelBlueGreenRedReserved8BitPerColor) {
		PRINT_ERROR(L"[!] Phos only supports the BGR8 pixel format, while the GOP mode uses a different pixel format.\r\n");
		goto Cleanup;
	}

	EFI_FILE_HANDLE FileHandle = NULL;

	if (EFI_ERROR(Status = OpenFile(LoadedImage, KERNEL_PATH, &FileHandle))) {
		PRINT_ERROR(L"[!] Failed to open a handle to the kernel image.\r\n");
		goto Cleanup;
	}

	UINTN BufferSize = 0;

	if (EFI_ERROR(Status = GetFileSize(FileHandle, &BufferSize))) {
		PRINT_ERROR(L"[!] Failed to get the kernel image size.\r\n");
		goto Cleanup;
	}

	UINTN    AllocatedSize = 0;
	VOID    *Image;
	FnKiMain KiMain;

	{
		VOID *RawImage = AllocatePool(BufferSize);

		Print(L"Reading kernel image...\r\n");

		if (EFI_ERROR(Status = FileHandle->Read(FileHandle, &BufferSize, RawImage))) {
			PRINT_ERROR(L"[!] Failed to read kernel image.\r\n");
			goto Cleanup;
		}
		
		FileHandle->Close(FileHandle);

		Print(L"Read %d bytes.\r\n", BufferSize);
		
		if (!Loader->ValidatePE64((CONST VOID *)RawImage, BufferSize)) {
			PRINT_ERROR(L"[!] Failed to load the kernel; invalid PE+ image.\r\n");
			goto Cleanup;
		}

		CONST IMAGE_DOS_HEADER   *DosHeader = (CONST IMAGE_DOS_HEADER *)RawImage;
		CONST IMAGE_NT_HEADERS64 *NtHeaders = (CONST IMAGE_NT_HEADERS64 *)((UINTN)RawImage + DosHeader->e_lfanew);

		Print(L"Loading kernel image...\r\n");

		Image = Loader->AllocateImage((CONST VOID *)RawImage, &AllocatedSize);
		KiMain = (FnKiMain)((UINTN)Image + NtHeaders->OptionalHeader.AddressOfEntryPoint);

		ASSERT(Image != NULL);

		Loader->MapSections(Image, (CONST VOID *)RawImage);
		Loader->RelocateImage(Image, &NtHeaders->OptionalHeader);

		FreePool(RawImage);
	}

	Print(L"Image loaded! Exiting boot services & calling entry point.\r\n");

	{
		UINTN MapKey = 0;

		if (EFI_ERROR(Status = GetMemoryMap(NULL, NULL, &MapKey, NULL, NULL))) {
			PRINT_ERROR(L"[!] Failed to get memory map.\r\n");
			goto Cleanup;
		}

		if (EFI_ERROR(Status = BS->ExitBootServices(ImageHandle, MapKey))) {
			PRINT_ERROR(L"[!] Failed to exit boot services.\r\n");
			goto Cleanup;
		}
	}
	
	KINIT_VID_INFO Video = {
		.Framebuffer  = (VOID *)GOP->Mode->FrameBufferBase,
		.BitsPerPixel = 32,
		.Width        = GOP->Mode->Info->HorizontalResolution,
		.Height       = GOP->Mode->Info->VerticalResolution
	};

	KiMain(&Video);

	return EFI_SUCCESS; // this shouldn't be reached

Cleanup:
	Print(L"-> Status = 0x%x\r\n", Status);

	if (Image != NULL)
		Loader->FreeImage(Image, AllocatedSize);
	if (FileHandle != NULL)
		FileHandle->Close(FileHandle);

	WAIT_FOR_EVENT(&ST->ConIn->WaitForKey);

	return Status;
}