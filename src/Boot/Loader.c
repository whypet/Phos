#include "Loader.h"
#include "Memory.h"
#include "String.h"
#include "Debug.h"

BOOLEAN
ValidatePE64(
	IN const VOID *RawImage,
	IN UINTN       Size
) {
	ASSERT(RawImage != NULL && Size > 0);
	
	const IMAGE_DOS_HEADER *DosHeader = (const IMAGE_DOS_HEADER *)RawImage;

	if (DosHeader->e_magic != DOS_MZ)
		return FALSE;
	
	const IMAGE_NT_HEADERS64 *NtHeaders = (const IMAGE_NT_HEADERS64 *)((UINTN)RawImage + DosHeader->e_lfanew);

	return NtHeaders->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64;
}

VOID *
AllocateImage(
	IN  const VOID *RawImage,
	OUT UINTN      *AllocatedSize
) {
	EFI_STATUS Status = EFI_SUCCESS;

	ASSERT(RawImage != NULL && AllocatedSize != NULL);

	const IMAGE_DOS_HEADER   *DosHeader = (const IMAGE_DOS_HEADER *)RawImage;
	const IMAGE_NT_HEADERS64 *NtHeaders = (const IMAGE_NT_HEADERS64 *)((UINTN)RawImage + DosHeader->e_lfanew);

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
	return EFI_ERROR(BS->FreePages((EFI_PHYSICAL_ADDRESS)Image, Size / 4096));
}

VOID
MapSections(
	OUT VOID       *Image,
	IN  const VOID *RawImage
) {
	ASSERT(Image != NULL && RawImage != NULL);

	const IMAGE_DOS_HEADER   *DosHeader = (const IMAGE_DOS_HEADER *)RawImage;
	const IMAGE_NT_HEADERS64 *NtHeaders = (const IMAGE_NT_HEADERS64 *)((UINTN)RawImage + DosHeader->e_lfanew);

	IMAGE_SECTION_HEADER *Section = IMAGE_FIRST_SECTION(NtHeaders);
	UINT16 Sections = NtHeaders->FileHeader.NumberOfSections;

	for (; Sections-- > 0; Section++) {
		CopyMemory(
			(UINT8*)Image + Section->VirtualAddress,
			(UINT8*)RawImage + Section->PointerToRawData,
			Section->SizeOfRawData);
	}
}

BOOLEAN
RelocateImage(
	OUT VOID                          *Image,
	IN  const IMAGE_OPTIONAL_HEADER64 *OptionalHeader
) {
	UINTN BaseDelta = (UINTN)Image - (UINTN)OptionalHeader->ImageBase;

	if (BaseDelta == 0)
		return FALSE;

	const IMAGE_DATA_DIRECTORY *RelocationDirectory = &OptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];

	if (RelocationDirectory->Size == 0)
		return FALSE;

	__unaligned IMAGE_BASE_RELOCATION *RelocationData = (__unaligned IMAGE_BASE_RELOCATION *)((UINTN)Image + RelocationDirectory->VirtualAddress);
	
	// Loop through every block
	for (; RelocationData->VirtualAddress > 0;) {
		UINT32 Count = (RelocationData->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(UINT16);
		IMAGE_BASE_RELOCATION_ENTRY *Entry = (IMAGE_BASE_RELOCATION_ENTRY *)(RelocationData + 1);

		for (; Count-- > 0; Entry++) {
			if (Entry->Type == IMAGE_REL_BASED_DIR64) {
				UINTN *PatchPointer = (UINTN *)((UINTN)Image + RelocationData->VirtualAddress + Entry->Offset);

				*PatchPointer += BaseDelta;
			}
		}

		RelocationData = (__unaligned IMAGE_BASE_RELOCATION *)((UINTN)RelocationData + RelocationData->SizeOfBlock);
	}

	return TRUE;
}

const LOADER Loader = {
	.ValidatePE64  = ValidatePE64,
	.AllocateImage = AllocateImage,
	.FreeImage     = FreeImage,
	.MapSections   = MapSections,
	.RelocateImage = RelocateImage
};