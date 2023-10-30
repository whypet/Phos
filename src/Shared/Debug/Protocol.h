#pragma once

#include <Phosdef.h>

#define DEBUG_SIGNATURE      "PHOSDB\0"
#define DEBUG_SIGNATURE_SIZE 8

#define DEBUG_PACKET_MESSAGE_ID 1

#define _DEBUG_PACKET    \
	CHAR8  Signature[8]; \
	UINT64 Size;         \
	UINT8  Identifier

#define DEBUG_PACKET(Packet)                                                \
	do {                                                                    \
		memcpy(&(Packet)->Signature, DebugSignature, DEBUG_SIGNATURE_SIZE); \
		(Packet)->Size = sizeof(DEBUG_PACKET);                              \
	} while (0)

#define DEBUG_PACKET_MESSAGE(Packet, _Message)            \
	do {                                                  \
		DEBUG_PACKET(Packet);                             \
		(Packet)->Identifier  = DEBUG_PACKET_MESSAGE_ID;  \
		if ((_Message) != NULL)                           \
			(Packet)->MessageSize = strlen(_Message) + 1; \
		(Packet)->Message     = (_Message);               \
		(Packet)->Size        =                           \
			OFFSET_OF(DEBUG_PACKET_MESSAGE, Message) +    \
			(Packet)->MessageSize;                        \
	} while (0)

#pragma pack(push, 1)

typedef struct {
	_DEBUG_PACKET;
} DEBUG_PACKET;

typedef struct {
	_DEBUG_PACKET;
	UINT64       MessageSize;
	CONST CHAR8 *Message;
} DEBUG_PACKET_MESSAGE;

#pragma pack(pop)

typedef struct {
	UINTN(SHAREDAPI * SerializeMessage)(
		IN     CONST DEBUG_PACKET_MESSAGE *Packet,
		IN OUT VOID                       *Data,
		IN     UINTN                       DataSize
	);

	UINTN(SHAREDAPI * DeserializeMessage)(
		IN     CONST VOID           *Data,
		IN     UINTN                 DataSize,
		IN OUT DEBUG_PACKET_MESSAGE *Packet
	);
} DEBUG_PROTOCOL;

EXTERN_C CONST CHAR8 DebugSignature[DEBUG_SIGNATURE_SIZE];

EXTERN_C CONST DEBUG_PROTOCOL *DebugProtocol;