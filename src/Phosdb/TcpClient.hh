#pragma once

#include "Phosdb.hh"

namespace Phosdb {
class TcpClient {
private:
	static WSADATA WsaData;

	std::wstring IpAddress;
	UINT16       Port;
	SOCKET       Socket;
public:
	static BOOL Initialize();
	static VOID Destroy();

	TcpClient(
		const std::wstring &IpAddress,
		UINT16              Port
	);

	BOOL Receive(
		std::vector<UINT8> &Data,
		UINTN              *BytesReceived
	) const;

	inline BOOL IsValid() const {
		return Socket != INVALID_SOCKET;
	}

	~TcpClient();
};
}