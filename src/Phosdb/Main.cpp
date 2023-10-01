#include "Phosdb.h"

INT
WINAPI
wWinMain(
	HINSTANCE Instance,
	HINSTANCE PrevInstance,
	PWSTR     CmdLine,
	INT       CmdShow
) {
	Window::Initialize();

	Window Wnd(L"Phosdb", 1024, 576);

	Wnd.Show();

	while (Wnd.IsValid() && Wnd.PollMessages()) ;

	return 0;
}