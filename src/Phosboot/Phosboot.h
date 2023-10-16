#pragma once

#include <Uefi.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>
#include <Guid/FileInfo.h>

#define BLOCKS(x, Size) (((x) - 1) / (Size) + 1)
#define ALIGN(x, Size) (BLOCKS(x, Size) * (Size))

#define KERNEL_PATH L"System\\Phoskrnl"

typedef struct {
	VOID  *Framebuffer;
	UINTN  Size;
	UINT32 Width;
	UINT32 Height;
} VIDEO_INFO;

typedef VOID(__attribute__((__ms_abi__)) * FnKiMain)(
	IN VIDEO_INFO
);

extern EFI_SYSTEM_TABLE     *ST;
extern EFI_BOOT_SERVICES    *BS;
extern EFI_RUNTIME_SERVICES *RS;