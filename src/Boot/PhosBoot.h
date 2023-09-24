#pragma once

#include <Uefi.h>
#include <Protocol/LoadedImage.h>

#define EXTEND(x, y) ((((x) - 1) / (y) + 1) * (y))

extern EFI_SYSTEM_TABLE     *ST;
extern EFI_BOOT_SERVICES    *BS;
extern EFI_RUNTIME_SERVICES *RS;