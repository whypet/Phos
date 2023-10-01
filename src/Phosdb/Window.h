#pragma once

#include "Phosdb.h"

class Window {
private:
	static WNDCLASSEXW WindowClass;
	static ATOM        Atom;
	
	HWND WindowHandle;

	static LRESULT WndProc(
		HWND   WindowHandle,
		UINT32 Message,
		WPARAM WParam,
		LPARAM LParam
	);
public:
	static BOOL Initialize();

	Window(PCWSTR Title, INT32 Width, INT32 Height);

	__forceinline HWND GetNativeHandle() {
		return WindowHandle;
	}

	__forceinline BOOL IsValid() {
		return WindowHandle != NULL;
	}

	__forceinline BOOL IsOpen() {
		return IsValid() && IsWindowVisible(WindowHandle);
	}

	__forceinline VOID Show() {
		ShowWindow(WindowHandle, SW_SHOW);
	}

	__forceinline VOID Close() {
		CloseWindow(WindowHandle);
	}

	BOOL PollMessages();

	~Window();
};