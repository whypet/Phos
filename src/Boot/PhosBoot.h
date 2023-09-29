#pragma once

#include <Uefi.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>
#include <Guid/FileInfo.h>

#define BLOCKS(x, Size) (((x) - 1) / (Size) + 1)
#define ALIGN(x, Size) (BLOCKS(x, Size) * (Size))

#define KERNEL_FILENAME L"Phoskrnl"

extern EFI_SYSTEM_TABLE     *ST;
extern EFI_BOOT_SERVICES    *BS;
extern EFI_RUNTIME_SERVICES *RS;