#include "TcpClient.hh"
#include "Logger.hh"

namespace Phosdb {
WSADATA TcpClient::WsaData = { 0 };

BOOL TcpClient::Initialize() {
	return WSAStartup(MAKEWORD(2, 2), &WsaData) == 0;
}

VOID TcpClient::Destroy() {
	WSACleanup();
}

TcpClient::TcpClient(
	const std::wstring &IpAddress,
	UINT16              Port
) {
	INT Result;

	this->IpAddress = IpAddress;
	this->Port      = Port;

	ADDRINFOW *AddrInfo = NULL;
	ADDRINFOW  Hints    = { 0 };

	Hints.ai_family   = AF_INET;
	Hints.ai_socktype = SOCK_STREAM;
	Hints.ai_protocol = IPPROTO_TCP;

	LOG(Trace, "Getting address info");

	Result = GetAddrInfoW(
		IpAddress.c_str(),
		std::to_wstring(Port).c_str(),
		&Hints,
		&AddrInfo);
	
	if (Result != 0) {
		LOG(Error,
			"WinSock2: Failed to get address info.\nError code: %d",
			WSAGetLastError());

		return;
	}
	
	LOG(Trace, "Creating socket");
	
	Socket = socket(AddrInfo->ai_family, AddrInfo->ai_socktype, AddrInfo->ai_protocol);

	if (Socket == INVALID_SOCKET) {
		LOG(Error,
			"WinSock2: Failed to create socket.\nError code: %d",
			WSAGetLastError());

		FreeAddrInfoW(AddrInfo);

		return;
	}

	ADDRINFOW *NextAddrInfo = AddrInfo;

	do {
		SOCKADDR_IN *AddrIn = reinterpret_cast<SOCKADDR_IN *>(NextAddrInfo->ai_addr);
		WCHAR Address[16] = { 0 };

		RtlIpv4AddressToStringW(&AddrIn->sin_addr, Address);
		
		LOG(Trace, "Trying to connect to %S", Address);

		Result = connect(
			Socket,
			NextAddrInfo->ai_addr,
			(INT)NextAddrInfo->ai_addrlen);
		
	 	NextAddrInfo = NextAddrInfo->ai_next;
	} while (Result == SOCKET_ERROR && NextAddrInfo != NULL);

	FreeAddrInfoW(AddrInfo);

	if (Result == SOCKET_ERROR) {
		closesocket(Socket);
		Socket = INVALID_SOCKET;
	}

	if (Socket == INVALID_SOCKET) {
		LOG(Error,
			"WinSock2: Failed to connect to socket.\nError code: %d",
			WSAGetLastError());
	}
}

TcpClient::~TcpClient() {
	if (Socket != INVALID_SOCKET) {
		shutdown(Socket, SD_BOTH);
		closesocket(Socket);
	}
}
}