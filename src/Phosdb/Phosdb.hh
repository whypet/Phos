#pragma once

#include <Phosdef.h>

#include <stdarg.h>
#include <stdio.h>

#include <algorithm>
#include <iterator>
#include <cstring>
#include <format>
#include <string>
#include <vector>

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
#include <imgui_stdlib.h>
#include <imgui_freetype.h>

#define EMSP "\u2003"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND WindowHandle, UINT32 Message, WPARAM WParam, LPARAM LParam);