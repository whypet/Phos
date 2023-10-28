#pragma once

#include <Phosdb.hh>

#include <Debug/Protocol.h>

#include "TcpClient.hh"

namespace Phosdb::Clients {
template<typename T> class SerialClient {
public:
	virtual T   &GetInnerClient();
	virtual BOOL IsValid()     const = 0;
	virtual BOOL IsConnected() const = 0;

	virtual BOOL SendMsg(
		const std::string &Message
	) = 0;
};

template<> class SerialClient<TcpClient> {
private:
	TcpClient Tcp;
public:
	SerialClient(
		TcpClient &Tcp
	);

	inline TcpClient &GetInnerClient() {
		return Tcp;
	}

	inline BOOL IsValid() const {
		return Tcp.IsValid();
	}

	inline BOOL IsConnected() const {
		return Tcp.IsConnected();
	}

	BOOL ReceivePackets(
		std::vector<std::vector<UINT8>> &Packets
	);

	VOID ParsePackets(
		const std::vector<std::vector<UINT8>> &Packets,
		std::vector<std::string>              &Log
	) const;

	BOOL SendMsg(
		const std::string &Message
	);
};
}