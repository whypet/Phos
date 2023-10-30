#pragma once

#include <Phosbase.h>

#define _PHOSKRNL_KD_H_

PHOSKRNLAPI
BOOLEAN
PHOSAPI
KdPrint(
	IN CONST CHAR8 *Message
);