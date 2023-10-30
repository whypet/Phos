#pragma once

#include <Phosdef.h>

typedef struct {
	VOLATILE VOID *Framebuffer;
	UINT32         Width;
	UINT32         Height;
	UINT32         BitsPerPixel;
} KINIT_VID_INFO;

typedef VOID(PHOSAPI * FnKiMain)(
	IN KINIT_VID_INFO *
);