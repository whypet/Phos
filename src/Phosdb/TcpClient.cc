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
	INT32 Result;

	this->IpAddress = IpAddress;
	this->Port      = Port;

	ADDRINFOW *AddrInfo = NULL;
	ADDRINFOW  Hints    = { 0 };

	Hints.ai_family   = AF_INET;
	Hints.ai_socktype = SOCK_STREAM;
	Hints.ai_protocol = IPPROTO_TCP;

	LOG(Trace, "Getting Winsock address info.");

	Result = GetAddrInfoW(
		IpAddress.c_str(),
		std::to_wstring(Port).c_str(),
		&Hints,
		&AddrInfo);
	
	if (Result != 0) {
		LOG(Error,
			"Failed to get Winsock address info.\nError code: %d",
			WSAGetLastError());

		return;
	}
	
	LOG(Trace, "Creating socket");
	
	Socket = socket(AddrInfo->ai_family, AddrInfo->ai_socktype, AddrInfo->ai_protocol);

	if (Socket == INVALID_SOCKET) {
		LOG(Error,
			"Failed to create socket.\nError code: %d",
			WSAGetLastError());

		FreeAddrInfoW(AddrInfo);

		return;
	}

	ADDRINFOW *NextAddrInfo = AddrInfo;

	do {
		SOCKADDR_IN *AddrIn = reinterpret_cast<SOCKADDR_IN *>(NextAddrInfo->ai_addr);
		WCHAR Address[16] = { 0 };

		RtlIpv4AddressToStringW(&AddrIn->sin_addr, Address);
		
		LOG(Trace, "Trying to connect to %S...", Address);

		Result = connect(
			Socket,
			NextAddrInfo->ai_addr,
			(INT32)NextAddrInfo->ai_addrlen);
		
	 	NextAddrInfo = NextAddrInfo->ai_next;
	} while (Result == SOCKET_ERROR && NextAddrInfo != NULL);

	if (Result == SOCKET_ERROR) {
		UINT32 ErrorCode = WSAGetLastError();
		
		closesocket(Socket);

		WSASetLastError(ErrorCode);
		Socket = INVALID_SOCKET;
	}

	if (Socket == INVALID_SOCKET) {
		LOG(Error,
			"Failed to connect to socket.\nError code: %d",
			WSAGetLastError());

		FreeAddrInfoW(AddrInfo);

		return;
	}

	FreeAddrInfoW(AddrInfo);

	// Enable non-blocking mode
	ULONG Mode = 1;
	ioctlsocket(Socket, FIONBIO, &Mode);
}

BOOL TcpClient::Receive(
	std::vector<UINT8> &Data,
	UINTN              *BytesReceived
) const {
	UINT8 Buffer[4096];
	UINTN Result;

	if (BytesReceived != NULL)
		*BytesReceived = 0;

	do {
		Result = recv(Socket, reinterpret_cast<char *>(Buffer), sizeof(Buffer), 0);

		INT32 ErrorCode = WSAGetLastError();

		if (ErrorCode == WSAEWOULDBLOCK)
			return TRUE;

		if (Result == SOCKET_ERROR) {
			LOG(Error,
				"Failed to receive data from socket.\nError code: %d",
				ErrorCode);

			return FALSE;
		}

		if (Result > 0) {
			Data.insert(Data.begin(), Buffer, Buffer + Result);

			if (BytesReceived != NULL)
				*BytesReceived += Result;
		}
	} while (Result > 0);

	return TRUE;
}

TcpClient::~TcpClient() {
	if (Socket != INVALID_SOCKET) {
		shutdown(Socket, SD_BOTH);
		closesocket(Socket);
	}
}
}