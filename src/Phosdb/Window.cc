#include "Window.hh"

namespace Phosdb {
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
	if (This == NULL)
		return DefWindowProcW(WindowHandle, Message, WParam, LParam);

	if (This->Procedure != NULL && This->Procedure(WindowHandle, Message, WParam, LParam))
		return 0;

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

VOID Window::Destroy() {
	UnregisterClassW(WindowClass.lpszClassName, WindowClass.hInstance);
}

Window::Window(
	PCWSTR Title,
	INT32  Width,
	INT32  Height
) {
	RECT Rectangle = { 0, 0, Width, Height };

	if (!AdjustWindowRectEx(&Rectangle, WS_OVERLAPPEDWINDOW & ~WS_OVERLAPPED, FALSE, 0))
		return;

	WindowHandle = CreateWindowExW(
		0,
		reinterpret_cast<PCWSTR>(Atom),
		Title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		Rectangle.right - Rectangle.left,
		Rectangle.bottom - Rectangle.top,
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

Window::Window(
	PCWSTR  Title,
	INT32   Width,
	INT32   Height,
	WNDPROC Procedure
) : Window(Title, Width, Height)
{
	this->Procedure = Procedure;
}

RECT Window::GetRectangle() const {
	RECT Rectangle = { 0 };

	GetClientRect(WindowHandle, &Rectangle);

	return Rectangle;
}

VOID Window::PollMessages() const {
	MSG Message = { 0 };

	while (PeekMessageW(&Message, WindowHandle, 0, 0, PM_REMOVE)) {
		TranslateMessage(&Message); 
        DispatchMessageW(&Message); 
	}
}

Window::~Window() {
	PostMessageW(WindowHandle, WM_CLOSE, 0, 0);
}
}