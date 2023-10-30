#include "Protocol.h"

// Runtime functions
#if PHOSKRNL
#include <Rtl/Rtl.h>
#include <Intrinsics.h>
#else
#include <string.h>
#endif

// Logging
#if PHOSDB
#include <Logger.h>
#else
#define LOG(...)
#endif

UINTN
SHAREDAPI
SerializeMessage(
	IN     CONST DEBUG_PACKET_MESSAGE *Packet,
	IN OUT VOID                       *Data,
	IN     UINTN                       DataSize
) {
	CONST UINTN   MessageSizeField = OFFSET_OF(DEBUG_PACKET_MESSAGE, MessageSize);
	CONST UINTN   MessageField     = OFFSET_OF(DEBUG_PACKET_MESSAGE, Message);
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
	IN     CONST VOID           *Data,
	IN     UINTN                 DataSize,
	IN OUT DEBUG_PACKET_MESSAGE *Packet
) {
	CONST UINTN  MessageSizeField = OFFSET_OF(DEBUG_PACKET_MESSAGE, MessageSize);
	CONST UINTN  MessageField     = OFFSET_OF(DEBUG_PACKET_MESSAGE, Message);
	CONST UINT8 *_Data            = (CONST UINT8 *)Data;
	
	if (DataSize < sizeof(DEBUG_PACKET) || DataSize < MessageField) {
		LOG(Warn, "Malformed packet data: Too small (%d bytes).", DataSize);
		return PHOS_ERROR;
	}

	UINT8 Id = ((CONST DEBUG_PACKET *)Data)->Identifier;

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

CONST CHAR8 DebugSignature[DEBUG_SIGNATURE_SIZE] = DEBUG_SIGNATURE;

CONST DEBUG_PROTOCOL _DebugProtocol = {
	.SerializeMessage   = SerializeMessage,
	.DeserializeMessage = DeserializeMessage
};

CONST DEBUG_PROTOCOL *DebugProtocol = &_DebugProtocol;