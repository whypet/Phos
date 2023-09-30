#pragma once

#include <PhosBase.h>

typedef struct {
	UINT16 Limit;
	UINT64 Base;
} HAL_GDTR;

VOID
PHOSAPI
HalCreateGdtr(
	IN OUT HAL_GDTR *Descriptor
);