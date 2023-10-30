#pragma once

#include <limits.h>
#include <stddef.h>
#include <stdint.h>

// Useful macros
#define COPY_OFFSET_BY(Dst, Src, Size)                  \
	do {                                                \
		memcpy(Dst, Src, Size);                         \
		(Dst) = (typeof(Dst))((UINT8 *)(Dst) + (Size)); \
	} while (0)

#define TYPE_OF_FIELD(Type, Field) typeof(((Type *)0)->Field)
#define COUNT_OF(x)                (sizeof(x) / sizeof((x)[0]))
#define OFFSET_OF                  offsetof

// API macros
#define PHOSAPI __attribute__((__ms_abi__))

#if PHOSKRNL
#define SHAREDAPI PHOSAPI
#define PHOSKRNLAPI __declspec(dllexport)
#else
#define SHAREDAPI
#define PHOSKRNLAPI __declspec(dllimport)
#endif

// Portability across C and C++ code
#if __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif

#undef VOID

// Non-boolean error handling
#define PHOS_ERROR SIZE_MAX

// Parameter annotations
#define IN
#define OUT
#define OPTIONAL

// Boolean values
#define FALSE 0
#define TRUE  1

// Win32 style macros
#define FORCEINLINE __forceinline
#define UNALIGNED   __unaligned
#define VOLATILE    volatile
#define EXTERN      extern
#define INLINE      inline
#define PACKED      __attribute__((__packed__))
#define STATIC      static
#define CONST       const

// Types
typedef void      VOID;
typedef char      CHAR8;
typedef wchar_t   CHAR32;
typedef int8_t    INT8;
typedef int16_t   INT16;
typedef int32_t   INT32;
typedef int64_t   INT64;
typedef ptrdiff_t INTN;
typedef uint8_t   UINT8;
typedef uint16_t  UINT16;
typedef uint32_t  UINT32;
typedef uint64_t  UINT64;
typedef size_t    UINTN;

#if !PHOSKRNL && !_PHOS_
typedef int32_t BOOL;
#endif