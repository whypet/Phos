#pragma once

typedef void                   VOID;
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
#endif