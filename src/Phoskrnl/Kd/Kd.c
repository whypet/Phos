#include "Kd.h"

#include <Rtl/Rtl.h>
#include <Hal/Hal.h>
#include <Debug/Protocol.h>

BOOL
PHOSAPI
KdPrint(
	IN const CHAR8 *Message
) {
	DEBUG_PACKET_MESSAGE Packet = { 0 };

	DEBUG_PACKET_MESSAGE(&Packet, Message);

	UINTN Size = (UINTN)Packet.Size;

	CHAR8 Data[Size];

	RtlZeroMemory(Data, Size);

	UINTN Result = DebugProtocol->SerializeMessage(
		&Packet,
		Data,
		Size);

	if (Result == 0)
		HalWriteSerialEx(COM1, (const UINT8 *)Data, Size);

	return Result == 0;
}