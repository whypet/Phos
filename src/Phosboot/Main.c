#include "Phosboot.h"
#include "Debug.h"
#include "Loader.h"
#include "Memory.h"
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
OpenFile(
	IN  EFI_LOADED_IMAGE *LoadedImage,
	IN  const CHAR16     *Filename,
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

	EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP = NULL;

	if (EFI_ERROR(Status = BS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID**)&GOP))) {
		Print(L"failed to get GOP.\r\n");
		goto Cleanup;
	}

	EFI_FILE_HANDLE FileHandle = NULL;

	if (EFI_ERROR(Status = OpenFile(LoadedImage, KERNEL_PATH, &FileHandle))) {
		Print(L"failed to open a handle to the kernel image.\r\n");
		goto Cleanup;
	}

	UINTN BufferSize = 0;

	if (EFI_ERROR(Status = GetFileSize(FileHandle, &BufferSize))) {
		Print(L"failed to get the kernel image size.\r\n");
		goto Cleanup;
	}

	UINTN    AllocatedSize = 0;
	VOID    *Image;
	FnKiMain KiMain;

	{
		VOID *RawImage = AllocatePool(BufferSize);

		Print(L"reading kernel image...\r\n");

		if (EFI_ERROR(Status = FileHandle->Read(FileHandle, &BufferSize, RawImage))) {
			Print(L"failed to read kernel image.\r\n");
			goto Cleanup;
		}
		
		FileHandle->Close(FileHandle);

		Print(L"read %d bytes.\r\n", BufferSize);
		
		if (!Loader.ValidatePE64((const VOID *)RawImage, BufferSize)) {
			Print(L"failed to load the kernel; invalid pe+ image.\r\n");
			goto Cleanup;
		}

		const IMAGE_DOS_HEADER   *DosHeader = (const IMAGE_DOS_HEADER *)RawImage;
		const IMAGE_NT_HEADERS64 *NtHeaders = (const IMAGE_NT_HEADERS64 *)((UINTN)RawImage + DosHeader->e_lfanew);

		Print(L"loading kernel image...\r\n");

		Image = Loader.AllocateImage((const VOID *)RawImage, &AllocatedSize);
		KiMain = (FnKiMain)((UINTN)Image + NtHeaders->OptionalHeader.AddressOfEntryPoint);

		ASSERT(Image != NULL);

		Loader.MapSections(Image, (const VOID *)RawImage);
		Loader.RelocateImage(Image, &NtHeaders->OptionalHeader);

		FreePool(RawImage);
	}

	Print(L"image loaded! exiting boot services & calling entry point.\r\n");

	{
		UINTN MapKey = 0;

		if (EFI_ERROR(Status = GetMemoryMap(NULL, NULL, &MapKey, NULL, NULL))) {
			Print(L"failed to get memory map.\r\n");
			goto Cleanup;
		}

		if (EFI_ERROR(Status = BS->ExitBootServices(ImageHandle, MapKey))) {
			Print(L"failed to exit boot services.\r\n");
			goto Cleanup;
		}
	}
	
	VIDEO_INFO Video = {
		.Framebuffer = (VOID *)GOP->Mode->FrameBufferBase,
		.Size        = GOP->Mode->FrameBufferSize,
		.Width       = GOP->Mode->Info->HorizontalResolution,
		.Height      = GOP->Mode->Info->VerticalResolution
	};

	KiMain(Video);

	return EFI_SUCCESS; // this shouldn't be reached

Cleanup:
	if (EFI_ERROR(Status))
		Print(L"status: %x\r\n", Status);

	if (Image != NULL)
		Loader.FreeImage(Image, AllocatedSize);
	if (FileHandle != NULL)
		FileHandle->Close(FileHandle);

	WaitForEvent(&ST->ConIn->WaitForKey);

	return Status;
}