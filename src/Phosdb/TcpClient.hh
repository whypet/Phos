#pragma once

#include <Phosdb.hh>

namespace Phosdb {
class TcpClient {
private:
	static WSADATA WsaData;

	BOOL        Connected;
	std::string Address;
	UINT16      Port;
	SOCKET      Socket;
public:
	static BOOL Initialize();
	static VOID Destroy();

	TcpClient(
		const std::string &Address,
		UINT16             Port
	);

	UINTN Receive(
		std::vector<UINT8> &Data
	);

	UINTN Send(
		const std::vector<UINT8> &Data
	);

	UINTN Send(
		const UINT8 *Data,
		UINTN        Size
	);

	inline BOOL IsValid() const {
		return Socket != INVALID_SOCKET;
	}

	inline BOOL IsConnected() const {
		return Connected;
	}

	inline std::string GetAddress() const {
		return Address;
	}

	inline UINT16 GetPort() const {
		return Port;
	}

	~TcpClient();
};
}