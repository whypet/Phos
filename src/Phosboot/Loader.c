#include "Loader.h"
#include "Memory.h"
#include "String.h"
#include "Debug.h"

BOOLEAN
ValidatePE64(
	IN CONST VOID *RawImage,
	IN UINTN       Size
) {
	ASSERT(RawImage != NULL && Size > 0);
	
	CONST IMAGE_DOS_HEADER *DosHeader = (CONST IMAGE_DOS_HEADER *)RawImage;

	if (DosHeader->e_magic != DOS_MZ)
		return FALSE;
	
	CONST IMAGE_NT_HEADERS64 *NtHeaders = (CONST IMAGE_NT_HEADERS64 *)((UINTN)RawImage + DosHeader->e_lfanew);

	return NtHeaders->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64;
}

VOID *
AllocateImage(
	IN  CONST VOID *RawImage,
	OUT UINTN      *AllocatedSize
) {
	EFI_STATUS Status = EFI_SUCCESS;

	ASSERT(RawImage != NULL && AllocatedSize != NULL);

	CONST IMAGE_DOS_HEADER   *DosHeader = (CONST IMAGE_DOS_HEADER *)RawImage;
	CONST IMAGE_NT_HEADERS64 *NtHeaders = (CONST IMAGE_NT_HEADERS64 *)((UINTN)RawImage + DosHeader->e_lfanew);

	INTN PageCount = BLOCKS(NtHeaders->OptionalHeader.SizeOfImage, 4096);

	// No relocations required if using the same base address
	EFI_PHYSICAL_ADDRESS Address = NtHeaders->OptionalHeader.ImageBase;

	Status = BS->AllocatePages(
		AllocateAddress,
		EfiLoaderData,
		PageCount,
		&Address);

	if (!EFI_ERROR(Status)) {
		*AllocatedSize = PageCount * 4096;

		return (VOID *)Address;
	}

	Status = BS->AllocatePages(
		AllocateAnyPages,
		EfiLoaderData,
		PageCount,
		&Address);

	if (!EFI_ERROR(Status)) {
		*AllocatedSize = PageCount * 4096;

		return (VOID *)Address;
	}
	
	return NULL;
}

BOOLEAN
FreeImage(
	IN VOID *Image,
	IN UINTN Size
) {
	return !EFI_ERROR(BS->FreePages((EFI_PHYSICAL_ADDRESS)Image, Size / 4096));
}

VOID
MapSections(
	IN OUT VOID       *Image,
	IN     CONST VOID *RawImage
) {
	ASSERT(Image != NULL && RawImage != NULL);

	CONST IMAGE_DOS_HEADER   *DosHeader = (CONST IMAGE_DOS_HEADER *)RawImage;
	CONST IMAGE_NT_HEADERS64 *NtHeaders = (CONST IMAGE_NT_HEADERS64 *)((UINTN)RawImage + DosHeader->e_lfanew);

	IMAGE_SECTION_HEADER *Section = IMAGE_FIRST_SECTION(NtHeaders);
	UINT16 Sections = NtHeaders->FileHeader.NumberOfSections;

	for (; Sections-- > 0; Section++) {
		CopyMemory(
			(UINT8 *)Image + Section->VirtualAddress,
			(UINT8 *)RawImage + Section->PointerToRawData,
			Section->SizeOfRawData);
	}
}

BOOLEAN
RelocateImage(
	IN OUT VOID                          *Image,
	IN     CONST IMAGE_OPTIONAL_HEADER64 *OptionalHeader
) {
	UINT64 BaseDelta = (UINT64)Image - OptionalHeader->ImageBase;

	if (BaseDelta == 0)
		return FALSE;

	CONST IMAGE_DATA_DIRECTORY *RelocationDirectory = &OptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];

	if (RelocationDirectory->Size == 0)
		return FALSE;

	UNALIGNED IMAGE_BASE_RELOCATION *RelocationData = (UNALIGNED IMAGE_BASE_RELOCATION *)((UINTN)Image + RelocationDirectory->VirtualAddress);
	
	// Loop through every block
	for (; RelocationData->VirtualAddress > 0;) {
		UINT32 Count = (RelocationData->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(UINT16);
		IMAGE_BASE_RELOCATION_ENTRY *Entry = (IMAGE_BASE_RELOCATION_ENTRY *)(RelocationData + 1);

		for (; Count-- > 0; Entry++) {
			VOID *Patch = (VOID *)((UINTN)Image + RelocationData->VirtualAddress + Entry->Offset);

			switch (Entry->Type) {
			case IMAGE_REL_BASED_DIR64:
				*(UINT64 *)Patch += BaseDelta;
				break;
			}
		}

		RelocationData = (UNALIGNED IMAGE_BASE_RELOCATION *)((UINTN)RelocationData + RelocationData->SizeOfBlock);
	}

	return TRUE;
}

CONST LOADER _Loader = {
	.ValidatePE64  = ValidatePE64,
	.AllocateImage = AllocateImage,
	.FreeImage     = FreeImage,
	.MapSections   = MapSections,
	.RelocateImage = RelocateImage
};

CONST LOADER *Loader = &_Loader;