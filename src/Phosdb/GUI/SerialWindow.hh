#pragma once

#include "Window.hh"

#include <TcpClient.hh>

namespace Phosdb::GUI {
class SerialWindow : public Window {
private:
	const Phosdb::TcpClient *Client;
	std::string Message;
	std::string Output;
public:
	SerialWindow(
		const Phosdb::TcpClient &Client
	);

	VOID Render();
};
}