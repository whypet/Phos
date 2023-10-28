#include "Protocol.h"

#ifdef PHOSDB

#include <Logger.h>

#include <string.h>
#include <stdio.h>

#else

#define LOG(...)

#ifdef PHOSKRNL

#include <Rtl/Rtl.h>
#include <Intrinsics.h>

#endif

#endif

UINTN
SHAREDAPI
SerializeMessage(
	IN     const DEBUG_PACKET_MESSAGE *Packet,
	IN OUT VOID                       *Data,
	IN     UINTN                       DataSize
) {
	const UINTN   MessageSizeField = OFFSET_OF(DEBUG_PACKET_MESSAGE, MessageSize);
	const UINTN   MessageField     = OFFSET_OF(DEBUG_PACKET_MESSAGE, Message);
	DEBUG_PACKET *PacketData       = (DEBUG_PACKET *)Data;
	UINT8        *_Data            = (UINT8 *)Data;

	if (DataSize < sizeof(DEBUG_PACKET) || DataSize < MessageField + Packet->MessageSize) {
		LOG(Warn, "Invalid buffer given: Buffer size is %zu bytes but at least %llu bytes were expected.",
			DataSize, (UINT64)MessageField + Packet->MessageSize);
		return PHOS_ERROR;
	}

	if (Packet->Identifier != DEBUG_PACKET_MESSAGE_ID) {
		LOG(Warn, "Invalid packet structure: Expected packet identifier %hhu but got %hhu.",
			DEBUG_PACKET_MESSAGE_ID, Packet->Identifier);
		return PHOS_ERROR;
	}

	memcpy(PacketData->Signature, DebugSignature, DEBUG_SIGNATURE_SIZE);
	PacketData->Size       = MessageField + Packet->MessageSize;
	PacketData->Identifier = DEBUG_PACKET_MESSAGE_ID;

	_Data += MessageSizeField;

	COPY_OFFSET_BY(_Data, &Packet->MessageSize, sizeof(TYPE_OF_FIELD(DEBUG_PACKET_MESSAGE, MessageSize)));
	if (Packet->Message != NULL)
		COPY_OFFSET_BY(_Data, Packet->Message, Packet->MessageSize);

	return 0;
}

UINTN
SHAREDAPI
DeserializeMessage(
	IN     const VOID           *Data,
	IN     UINTN                 DataSize,
	IN OUT DEBUG_PACKET_MESSAGE *Packet
) {
	const UINTN  MessageSizeField = OFFSET_OF(DEBUG_PACKET_MESSAGE, MessageSize);
	const UINTN  MessageField     = OFFSET_OF(DEBUG_PACKET_MESSAGE, Message);
	const UINT8 *_Data            = (const UINT8 *)Data;
	
	if (DataSize < sizeof(DEBUG_PACKET) || DataSize < MessageField) {
		LOG(Warn, "Malformed packet data: Too small (%d bytes).", DataSize);
		return PHOS_ERROR;
	}

	UINT8 Id = ((const DEBUG_PACKET *)Data)->Identifier;

	if (Id != DEBUG_PACKET_MESSAGE_ID) {
		LOG(Warn, "Malformed packet data: Expected packet identifier %d but got %d.", DEBUG_PACKET_MESSAGE_ID, Id);
		return PHOS_ERROR;
	}

#define X TYPE_OF_FIELD(DEBUG_PACKET_MESSAGE, MessageSize)
	X MessageSize = *(X*)(_Data + MessageSizeField);
#undef X

	if (Packet->Message == NULL) {
		Packet->MessageSize = MessageSize;

		return 1;
	} else {
		if (Packet->MessageSize < MessageSize) {
			LOG(Warn, "Message of %llu bytes in DEBUG_PACKET_MESSAGE structure doesn't have enough data for message of %llu bytes in packet data.",
				(UINT64)Packet->MessageSize, (UINT64)MessageSize);
		}

		memcpy((VOID *)Packet->Message, _Data + MessageField, Packet->MessageSize);

		return 0;
	}
}

const CHAR8 DebugSignature[DEBUG_SIGNATURE_SIZE] = DEBUG_SIGNATURE;

const DEBUG_PROTOCOL _DebugProtocol = {
	.SerializeMessage   = SerializeMessage,
	.DeserializeMessage = DeserializeMessage
};

const DEBUG_PROTOCOL *DebugProtocol = &_DebugProtocol;