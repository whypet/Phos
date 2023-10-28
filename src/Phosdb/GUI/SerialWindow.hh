#pragma once

#include "Window.hh"

#include <Clients/SerialClient.hh>

namespace Phosdb::GUI {
template<typename T> class SerialWindow : public Window {
private:
	Phosdb::Clients::SerialClient<T> Serial;

	BOOL CanUpdate = TRUE;
	std::string Message;
	std::string Log;

	VOID RenderClientInfo();
public:
	SerialWindow(
		Phosdb::Clients::SerialClient<T> &Serial
	);

	VOID Update();
	VOID Render();
};
}