#pragma once

#include <Phosdef.h>

#if !defined(_MSC_VER) || defined(__clang__)

__forceinline
UINT8
__inbyte(
	IN UINT16 Port
) {
	UINT8 Value;

	__asm__ __volatile__(
		"inb %1, %0"
		: "=a" (Value)
		: "Nd" (Port)
		: "memory");

	return Value;
}

__forceinline
VOID
__outbyte(
	IN UINT16 Port,
	IN UINT8  Value
) {
	__asm__ __volatile__(
		"outb %0, %1"
		: : "a" (Value), "Nd" (Port)
		: "memory");
}

#endif

#ifdef PHOSKRNL

#ifndef _PHOSKRNL_RTL_H_

inline
EXPORT
VOID
PHOSAPI
RtlSetMemory(
	IN OUT VOID *Dst,
	IN     UINT8 Val,
	IN     UINTN Size
);

inline
EXPORT
VOID
PHOSAPI
RtlCopyMemory(
	IN OUT VOID       *Dst,
	IN     const VOID *Src,
	IN     UINTN       Size
);

#endif

__forceinline
EXPORT
VOID *
memset(
	IN OUT VOID *Dst,
	IN     INT32 Val,
	IN     UINTN Size
) {
	RtlSetMemory(Dst, Val, Size);

	return Dst;
}

__forceinline
EXPORT
VOID *
memcpy(
	IN OUT VOID       *Dst,
	IN     const VOID *Src,
	IN     UINTN       Size
) {
	RtlCopyMemory(Dst, Src, Size);

	return Dst;
}

#endif