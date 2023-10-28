#include "SerialClient.hh"

#include <Logger.hh>

static BOOL SerializeMsgPacket(
	DEBUG_PACKET_MESSAGE *Packet,
	std::vector<UINT8>   &Data
) {
	UINTN              Result;
	std::vector<UINT8> Temp;

	Temp.resize(Packet->Size);
	
	Result = DebugProtocol->SerializeMessage(
		Packet,
		&Temp[0],
		Packet->Size);

	if (Result == 0)
		Data.insert(Data.end(), Temp.begin(), Temp.end());

	return Result == 0;
}

namespace Phosdb::Clients {
SerialClient<TcpClient>::SerialClient(
	TcpClient &Tcp
) : Tcp(Tcp) { }

BOOL SerialClient<TcpClient>::ReceivePackets(
	std::vector<std::vector<UINT8>> &Packets
) {
	BOOL PacketCutoff = FALSE;
	std::vector<UINT8> Data;

	do {
		UINTN Caret  = Data.size();
		UINTN Result;

		do {
			Result = Tcp.Receive(Data);
		} while (Result != 0 && Result != PHOS_ERROR);
		
		if (Result == PHOS_ERROR)
			return FALSE;
		else if (Data.size() - Caret == 0)
			return TRUE;
Check:
		auto Iterator = std::search(
			Data.begin() + Caret, Data.end(),
			std::begin(DebugSignature), std::end(DebugSignature));

		if (Iterator != Data.end()) {
			INTN          Index  = std::distance(Data.begin(), Iterator);
			// Not dereferenced until we're sure a packet fits in our data
			DEBUG_PACKET *Packet = reinterpret_cast<DEBUG_PACKET *>(&Data[Index]);

			assert(Index >= 0);

			if (Data.size() - Index < sizeof(DEBUG_PACKET) || Data.size() - Index < Packet->Size)
				PacketCutoff = TRUE;
			else {
				PacketCutoff = FALSE;

				Packets.push_back(std::vector<UINT8>(
					Data.begin() + Index, Data.begin() + Index + Packet->Size));

				Caret += Packet->Size;
				if (Caret < Data.size())
					goto Check;
			}
		}
	} while (PacketCutoff);

	return TRUE;
}

VOID SerialClient<TcpClient>::ParsePackets(
	const std::vector<std::vector<UINT8>> &Packets,
	std::vector<std::string>              &Log
) const {
	for (std::vector<UINT8> Data : Packets) {
		if (Data.size() < sizeof(DEBUG_PACKET)) {
			LOG(Warn, "Packet size of %d bytes is smaller than the DEBUG_PACKET structure of %d bytes.",
				Data.size(), sizeof(DEBUG_PACKET));
			continue;
		}

		const DEBUG_PACKET *Packet =
			const_cast<const DEBUG_PACKET *>(reinterpret_cast<DEBUG_PACKET *>(&Data[0]));

		if (Packet->Size != Data.size()) {
			LOG(Warn, "Packet size of %d bytes doesn't match structure member DEBUG_PACKET::Size = %d.",
				Data.size(), Packet->Size);
			continue;
		}

		struct tm LocalTime = { 0 };
		time_t    Time      = 0;

		time(&Time);
		localtime_s(&LocalTime, &Time);

		switch (Packet->Identifier) {
		default:
			LOG(Warn, "Invalid packet identifier %d.", Packet->Identifier);
			continue;
		case DEBUG_PACKET_MESSAGE_ID: {
			DEBUG_PACKET_MESSAGE MessagePacket = { };
			UINTN                Result;

			Result = DebugProtocol->DeserializeMessage(
				&Data[0],
				Data.size(),
				&MessagePacket);

			if (Result != 1)
				break;

			CHAR *Message = new CHAR[MessagePacket.MessageSize]();

			MessagePacket.Message = const_cast<const CHAR *>(Message);

			Result = DebugProtocol->DeserializeMessage(
				&Data[0],
				Data.size(),
				&MessagePacket);

			if (Result != 0) {
				delete[] Message;
				break;
			}

			std::string String = std::format(
				"[{:02}:{:02}:{:02}] {}\n",
				LocalTime.tm_hour, LocalTime.tm_min, LocalTime.tm_sec, MessagePacket.Message);

			Log.push_back(String);

			delete[] Message;
		}
		}
	}
}

BOOL SerialClient<TcpClient>::SendMsg(
	const std::string &Message
) {
	DEBUG_PACKET_MESSAGE Packet = { };

	DEBUG_PACKET_MESSAGE(&Packet, Message.c_str());

	std::vector<UINT8> Data;

	SerializeMsgPacket(&Packet, Data);

	INT BytesSent = Tcp.Send(
		const_cast<const VOID *>(reinterpret_cast<VOID *>(&Data[0])),
		Data.size());

	return BytesSent == Data.size();
}
}