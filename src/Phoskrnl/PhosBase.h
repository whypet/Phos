#pragma once

#include <Phosdef.h>
#include <Intrinsics.h>

typedef struct {
	volatile VOID *Framebuffer;
	UINTN          Size;
	UINT32         Width;
	UINT32         Height;
} VIDEO_INFO;