#pragma once

#include <stddef.h>

#define PHOSAPI __attribute__((__ms_abi__))

#define IN
#define OUT
#define OPTIONAL

#define FALSE 0
#define TRUE  1

typedef void                   VOID;
typedef unsigned char          BOOL;
typedef wchar_t                CHAR;
typedef unsigned char          CHAR8;
typedef signed char            INT8;
typedef signed short int       INT16;
typedef signed long int        INT32;
typedef signed long long int   INT64;
typedef unsigned char          UINT8;
typedef unsigned short int     UINT16;
typedef unsigned long int      UINT32;
typedef unsigned long long int UINT64;

// this should be true
#if __x86_64__
typedef signed long long int   INTN;
typedef unsigned long long int UINTN;
#else
#error Phos should compile under x86_64.

typedef signed long int   INTN;
typedef unsigned long int UINTN;
#endif

#include <Intrinsics.h>

typedef struct {
	volatile VOID *Framebuffer;
	UINTN          Size;
	UINT32         Width;
	UINT32         Height;
} VIDEO_INFO;