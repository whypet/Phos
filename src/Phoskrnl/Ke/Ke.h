#pragma once

#include <Phosbase.h>

#define _PHOSKRNL_KE_H_

VOID
PHOSAPI
KiSystemInit(
	VOID
);

VOID
PHOSAPI
KiHalt(
	VOID
);

PHOSKRNLAPI
VOID
PHOSAPI
KePanic(
	CONST CHAR8 *Reason
);