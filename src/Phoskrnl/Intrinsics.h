#pragma once

#include <PhosBase.h>

#if !defined(_MSC_VER) || defined(__clang__)
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
#endif