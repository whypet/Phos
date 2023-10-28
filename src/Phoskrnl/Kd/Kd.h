#pragma once

#include <Phosbase.h>

#define _PHOSKRNL_KD_H_

EXPORT
BOOL
PHOSAPI
KdPrint(
	IN const CHAR8 *Message
);