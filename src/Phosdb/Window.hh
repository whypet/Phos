#pragma once

#include "Phosdb.hh"

namespace Phosdb {
class Window {
private:
	static WNDCLASSEXW WindowClass;
	static ATOM        Atom;

	HWND    WindowHandle;
	WNDPROC Procedure;

	static LRESULT WndProc(
		HWND   WindowHandle,
		UINT32 Message,
		WPARAM WParam,
		LPARAM LParam
	);
public:
	static BOOL Initialize();
	static VOID Destroy();

	Window(
		PCWSTR Title,
		INT32  Width,
		INT32  Height
	);

	Window(
		PCWSTR  Title,
		INT32   Width,
		INT32   Height,
		WNDPROC Procedure
	);

	inline HWND GetNativeHandle() const {
		return WindowHandle;
	}

	inline BOOL IsValid() const {
		return WindowHandle != NULL;
	}

	inline BOOL IsOpen() const {
		return IsValid() && IsWindowVisible(WindowHandle);
	}

	inline VOID Show() const {
		ShowWindow(WindowHandle, SW_SHOW);
	}

	inline VOID Close() const {
		CloseWindow(WindowHandle);
	}

	RECT GetRectangle() const;

	inline INT32 X() const {
		return GetRectangle().left;
	}

	inline INT32 Y() const {
		return GetRectangle().top;
	}

	inline INT32 Width() const {
		RECT Rectangle = GetRectangle();

		return Rectangle.right - Rectangle.left;
	}

	inline INT32 Height() const {
		RECT Rectangle = GetRectangle();

		return Rectangle.bottom - Rectangle.top;
	}

	VOID PollMessages() const;

	~Window();
};
}