#pragma once

#include <stddef.h>
#include <stdint.h>
#include <limits.h>

#define COPY_OFFSET_BY(Dst, Src, Size)                  \
	do {                                                \
		memcpy(Dst, Src, Size);                         \
		(Dst) = (typeof(Dst))((UINT8 *)(Dst) + (Size)); \
	} while (0)

#define OFFSET_OF offsetof
#define TYPE_OF_FIELD(Type, Field) typeof(((Type *)0)->Field)

#if __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif

#ifndef _WINDOWS_

#define IN
#define OUT
#define OPTIONAL

#define FALSE 0
#define TRUE  1

typedef void VOID;
typedef int  BOOL;

#endif

#ifdef PHOSKRNL

#define SHAREDAPI PHOSAPI
#define PHOSAPI __attribute__((__ms_abi__))
#define EXPORT  __declspec(dllexport)

typedef wchar_t CHAR;

#else

#define SHAREDAPI

typedef char CHAR;

#endif

#define PHOS_ERROR SIZE_MAX

typedef char     CHAR8;
typedef wchar_t  WCHAR;
typedef int8_t   INT8;
typedef int16_t  INT16;
typedef int32_t  INT32;
typedef int64_t  INT64;
typedef uint8_t  UINT8;
typedef uint16_t UINT16;
typedef uint32_t UINT32;
typedef uint64_t UINT64;

#if __x86_64__
typedef signed long long int   INTN;
typedef unsigned long long int UINTN;
#else
typedef signed long int   INTN;
typedef unsigned long int UINTN;
#endif