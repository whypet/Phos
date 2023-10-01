#include "Window.h"

WNDCLASSEXW Window::WindowClass = { .cbSize = sizeof(WNDCLASSEXW) };
ATOM        Window::Atom        = 0;

LRESULT Window::WndProc(
	HWND   WindowHandle,
	UINT32 Message,
	WPARAM WParam,
	LPARAM LParam
) {
	INTN UserData = GetWindowLongPtrW(
		WindowHandle,
		GWLP_USERDATA);

	Window *This = reinterpret_cast<Window *>(UserData);

	switch (Message) {
	default:
		return DefWindowProcW(WindowHandle, Message, WParam, LParam);
	case WM_CLOSE:
		DestroyWindow(WindowHandle);

		break;
	case WM_DESTROY:
		PostQuitMessage(0);

		This->WindowHandle = NULL;

		break;
	}

	return 0;
}

BOOL Window::Initialize() {
	WindowClass.style         = CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc   = WndProc;
	WindowClass.hInstance     = GetModuleHandleW(NULL);
	WindowClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	WindowClass.lpszClassName = L"Phosdb";

	Atom = RegisterClassExW(&WindowClass);

	return Atom != 0;
}

Window::Window(PCWSTR Title, INT32 Width, INT32 Height) {
	WindowHandle = CreateWindowExW(
		0,
		reinterpret_cast<PCWSTR>(Atom),
		Title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		Width,
		Height,
		NULL,
		NULL,
		WindowClass.hInstance,
		NULL);
	
	if (WindowHandle == NULL)
		return;

	SetWindowLongPtrW(
		WindowHandle,
		GWLP_USERDATA,
		reinterpret_cast<LONG_PTR>(this));
}

BOOL Window::PollMessages() {
	BOOL Result;
	MSG  Message = { 0 };

	while ((Result = GetMessageW(&Message, WindowHandle, 0, 0)) != 0) {
		if (Result == -1)
			return FALSE;
		
		TranslateMessage(&Message); 
        DispatchMessageW(&Message); 
	}

	return TRUE;
}

Window::~Window() {
	PostMessageW(WindowHandle, WM_CLOSE, 0, 0);
}