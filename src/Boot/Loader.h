#pragma once

#include "PhosBoot.h"

#define DOS_MZ 0x5A4D

#define IMAGE_FILE_MACHINE_AMD64 0x8664
#define IMAGE_REL_BASED_DIR64 10

#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES 16

typedef struct {      // DOS .EXE header
    UINT16 e_magic;                     // Magic number
    UINT16 e_cblp;                      // Bytes on last page of file
    UINT16 e_cp;                        // Pages in file
    UINT16 e_crlc;                      // Relocations
    UINT16 e_cparhdr;                   // Size of header in paragraphs
    UINT16 e_minalloc;                  // Minimum extra paragraphs needed
    UINT16 e_maxalloc;                  // Maximum extra paragraphs needed
    UINT16 e_ss;                        // Initial (relative) SS value
    UINT16 e_sp;                        // Initial SP value
    UINT16 e_csum;                      // Checksum
    UINT16 e_ip;                        // Initial IP value
    UINT16 e_cs;                        // Initial (relative) CS value
    UINT16 e_lfarlc;                    // File address of relocation table
    UINT16 e_ovno;                      // Overlay number
    UINT16 e_res[4];                    // Reserved words
    UINT16 e_oemid;                     // OEM identifier (for e_oeminfo)
    UINT16 e_oeminfo;                   // OEM information; e_oemid specific
    UINT16 e_res2[10];                  // Reserved words
    INT32  e_lfanew;                    // File address of new exe header
} IMAGE_DOS_HEADER;

typedef struct {
    UINT32 VirtualAddress;
    UINT32 Size;
} IMAGE_DATA_DIRECTORY;

typedef struct {
    UINT16 Machine;
    UINT16 NumberOfSections;
    UINT32 TimeDateStamp;
    UINT32 PointerToSymbolTable;
    UINT32 NumberOfSymbols;
    UINT16 SizeOfOptionalHeader;
    UINT16 Characteristics;
} IMAGE_FILE_HEADER;

typedef struct {
    UINT16 Magic;
    UINT8  MajorLinkerVersion;
    UINT8  MinorLinkerVersion;
    UINT32 SizeOfCode;
    UINT32 SizeOfInitializedData;
    UINT32 SizeOfUninitializedData;
    UINT32 AddressOfEntryPoint;
    UINT32 BaseOfCode;
    UINT64 ImageBase;
    UINT32 SectionAlignment;
    UINT32 FileAlignment;
    UINT16 MajorOperatingSystemVersion;
    UINT16 MinorOperatingSystemVersion;
    UINT16 MajorImageVersion;
    UINT16 MinorImageVersion;
    UINT16 MajorSubsystemVersion;
    UINT16 MinorSubsystemVersion;
    UINT32 Win32VersionValue;
    UINT32 SizeOfImage;
    UINT32 SizeOfHeaders;
    UINT32 CheckSum;
    UINT16 Subsystem;
    UINT16 DllCharacteristics;
    UINT64 SizeOfStackReserve;
    UINT64 SizeOfStackCommit;
    UINT64 SizeOfHeapReserve;
    UINT64 SizeOfHeapCommit;
    UINT32 LoaderFlags;
    UINT32 NumberOfRvaAndSizes;
    IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER64;

typedef struct {
    UINT32                  Signature;
    IMAGE_FILE_HEADER       FileHeader;
    IMAGE_OPTIONAL_HEADER64 OptionalHeader;
} IMAGE_NT_HEADERS64;

typedef union {
	UINT16 Value;
	struct {
		UINT16 Type   : 4;
		UINT16 Offset : 12;
	};
} IMAGE_BASE_RELOCATION_ENTRY, *PIMAGE_BASE_RELOCATION_ENTRY;

typedef struct {
	BOOLEAN
	(*ValidatePE)(
		IN const VOID *Image,
		IN UINTN Size
	);

    /*
	VOID *(*AllocateRegion)(const VOID *Image, UINTN *AllocatedSize);
	BOOLEAN(*FreeRegion)(VOID *Region);
	VOID(*MapSections)(VOID *Region, const VOID *Image);
	BOOLEAN(*RelocateImage)(VOID *Region, IMAGE_OPTIONAL_HEADER64 *OptionalHeader);
	BOOLEAN(*FixImports)(VOID *Region, IMAGE_OPTIONAL_HEADER64 *OptionalHeader);
    */
} LOADER;

extern const LOADER Loader;