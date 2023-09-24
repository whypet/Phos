#include "Loader.h"
#include "Debug.h"

BOOLEAN
ValidatePE(
	IN const VOID *Image,
	IN UINTN Size
) {
	ASSERT(Image != NULL && Size > 0);

	if (Size < 0x1000)
		return FALSE;
	
	IMAGE_DOS_HEADER *DosHeader = (IMAGE_DOS_HEADER *)Image;
	if (DosHeader->e_magic != DOS_MZ)
		return FALSE;
	
	IMAGE_NT_HEADERS64 *NtHeaders = (IMAGE_NT_HEADERS64 *)((UINTN)Image + DosHeader->e_lfanew);

	return NtHeaders->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64;
}

const LOADER Loader = {
	.ValidatePE = ValidatePE
};