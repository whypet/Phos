#pragma once

#include <stdarg.h>
#include <stdio.h>

#include <string>
#include <locale>
#include <codecvt>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <wrl/client.h>
#include <d3d11.h>
#include <DirectXMath.h>

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <mstcpip.h>
#include <ip2string.h>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <freetype/imgui_freetype.h>

using INTN  = LONG_PTR;
using UINTN = ULONG_PTR;

#define WIDE_PREFIX(x) L ## #x

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND WindowHandle, UINT32 Message, WPARAM WParam, LPARAM LParam);