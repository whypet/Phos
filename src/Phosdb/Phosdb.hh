#pragma once

#include <Windows.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <dwmapi.h>
#include <stdarg.h>
#include <stdio.h>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <freetype/imgui_freetype.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND WindowHandle, UINT32 Message, WPARAM WParam, LPARAM LParam);

typedef LONG_PTR  INTN;
typedef ULONG_PTR UINTN;

typedef enum {
	Info,
	Warn,
	Error
} LOG;

VOID
Log(
	IN LOG    Type,
	IN PCWSTR Format,
	...
);