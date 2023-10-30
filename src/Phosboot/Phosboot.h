#pragma once

#include <Uefi.h>

#include <Phosdef.h>
#include <Phoskinit.h>

#include <Protocol/GraphicsOutput.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>
#include <Guid/FileInfo.h>

#define BLOCKS(x, Size) (((x) - 1) / (Size) + 1)
#define ALIGN(x, Size) (BLOCKS(x, Size) * (Size))

#define KERNEL_PATH L"System\\Phoskrnl"

EXTERN EFI_SYSTEM_TABLE     *ST;
EXTERN EFI_BOOT_SERVICES    *BS;
EXTERN EFI_RUNTIME_SERVICES *RS;