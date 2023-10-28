#include "TcpClient.hh"

#include <Logger.hh>
#include <Util.hh>

namespace Phosdb::Clients {
WSADATA TcpClient::WsaData = { 0 };

BOOL TcpClient::Initialize() {
	return WSAStartup(MAKEWORD(2, 2), &WsaData) == 0;
}

VOID TcpClient::Destroy() {
	WSACleanup();
}

TcpClient::TcpClient(
	const std::string &Address,
	UINT16             Port
) {
	INT32 Result;

	this->Address = Address;
	this->Port    = Port;
	Connected     = FALSE;

	ADDRINFOW *AddrInfo = NULL;
	ADDRINFOW  Hints    = { 0 };

	Hints.ai_family   = AF_INET;
	Hints.ai_socktype = SOCK_STREAM;
	Hints.ai_protocol = IPPROTO_TCP;

	LOG(Trace, "Getting Winsock address info.");

	Result = GetAddrInfoW(
		Util::Utf8ToWide(std::string(Address)).c_str(),
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

	Connected = TRUE;

	// Enable non-blocking mode
	ULONG Mode = 1;
	ioctlsocket(Socket, FIONBIO, &Mode);
}

UINTN TcpClient::Receive(
	std::vector<UINT8> &Data
) {
	UINT8 Buffer[4096];
	INT   Result;
	UINTN Bytes;

	Bytes = 0;

	do {
		Result = recv(Socket, reinterpret_cast<char *>(Buffer), sizeof(Buffer), 0);

		INT32 ErrorCode = WSAGetLastError();

		if (ErrorCode == WSAEWOULDBLOCK)
			break;

		if (Result == SOCKET_ERROR) {
			LOG(Error,
				"Failed to receive data from socket.\nError code: %d",
				ErrorCode);

			Connected = FALSE;

			return PHOS_ERROR;
		}

		if (Result > 0 && Result != SOCKET_ERROR) {
			Data.insert(Data.begin(), Buffer, Buffer + Result);
			Bytes += Result;
		}
	} while (Result > 0);

	Connected = TRUE;

	return Bytes;
}

UINTN TcpClient::Send(
	const std::vector<UINT8> &Data
) {
	return Send(&Data[0], Data.size());
}

UINTN TcpClient::Send(
	const VOID *Data,
	UINTN       Size
) {
	INT   Result;
	UINTN Bytes;

	Bytes = 0;

	do {
		FD_SET  WriteFd = { 0 };
		TIMEVAL Timeout = { .tv_sec = 5 };

		FD_SET(Socket, &WriteFd);

		Result = select(0, NULL, &WriteFd, NULL, &Timeout);

		if (Result == 0)
			return PHOS_ERROR;
		else if (Result == SOCKET_ERROR) {
			LOG(Error,
				"Failed to get socket writeability status.\nError code: %d",
				WSAGetLastError());

			Connected = FALSE;

			return PHOS_ERROR;
		}

		Result = send(Socket, reinterpret_cast<const CHAR *>(Data) + Bytes, Size - Bytes, 0);

		INT32 ErrorCode = WSAGetLastError();

		if (ErrorCode == WSAEWOULDBLOCK)
			continue;

		if (Result == SOCKET_ERROR) {
			LOG(Error,
				"Failed to send data to socket.\nError code: %d",
				ErrorCode);

			Connected = FALSE;

			return PHOS_ERROR;
		}

		Bytes += Result;
	} while (Bytes < Size);

	Connected = TRUE;

	return Bytes;
}

TcpClient::~TcpClient() {
	if (Socket != INVALID_SOCKET) {
		shutdown(Socket, SD_BOTH);
		closesocket(Socket);
	}
}
}