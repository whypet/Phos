#include "Phosdb.hh"
#include "Logger.hh"
#include "Direct3D.hh"
#include "TcpClient.hh"
#include "Window.hh"

class Main {
public:
	static INT32 Entry();
};

#ifdef WINMAIN
INT32
WINAPI
wWinMain(
	HINSTANCE Instance,
	HINSTANCE PrevInstance,
	PWSTR     CmdLine,
	INT32     CmdShow
) {
	return Entry();
}
#else
INT32 main() {
	return Main::Entry();
}
#endif

INT32 Main::Entry() {
	Phosdb::TcpClient::Initialize();

	Phosdb::TcpClient Client(L"localhost", 8888);

	LOG(Info, "Creating window...");

	Phosdb::Window::Initialize();

	Phosdb::Window Wnd(L"Phosdb", 1024, 576, ImGui_ImplWin32_WndProcHandler);

	if (!Wnd.IsValid()) {
		LOG(Error,
			"Failed to create window.\nError code: %d",
			GetLastError());

		return 1;
	}

	Phosdb::Direct3D D3D(Wnd);

	if (!D3D.IsValid()) {
		LOG(Error,
			"Failed to initialize Direct3D 11.\nHRESULT status: 0x%08x",
			D3D.GetLastResult());

		return 1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImFontConfig cfg;
	cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_MonoHinting;
	cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_Monochrome;
	cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_Bitmap;
	io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/tahoma.ttf", 11.0f, &cfg);

	ImGui::StyleColorsDark();

	ImGuiStyle *Style = &ImGui::GetStyle();

	Style->ScrollbarRounding = 0.0f;
	Style->WindowRounding    = 0.0f;
	Style->ChildRounding     = 0.0f;
	Style->FrameRounding     = 0.0f;
	Style->PopupRounding     = 0.0f;
	Style->GrabRounding      = 0.0f;
	Style->TabRounding       = 0.0f;

	for (INT32 i = 0; i < ImGuiCol_COUNT; i++) {
		ImVec4 *Color = &Style->Colors[i];

		DirectX::XMFLOAT4 Rgb(Color->x, Color->y, Color->z, Color->w);
		DirectX::XMFLOAT4 Hsv;

		DirectX::XMStoreFloat4(&Hsv, DirectX::XMColorRGBToHSV(DirectX::XMLoadFloat4(&Rgb)));

		Hsv.x = 0.45f;
		if (Hsv.z < 1.0f)
			Hsv.y += 0.25f;

		DirectX::XMStoreFloat4(&Rgb, DirectX::XMColorHSVToRGB(DirectX::XMLoadFloat4(&Hsv)));

		Color->x = Rgb.x;
		Color->y = Rgb.y;
		Color->z = Rgb.z;
		Color->w = Rgb.w;
	}

	ImGui_ImplWin32_Init(Wnd.GetNativeHandle());
	ImGui_ImplDX11_Init(D3D.Device.Get(), D3D.Context.Get());

	Wnd.Show();

	INT32 Width;
	INT32 Height;

	while (D3D.IsValid()) {
		Width  = Wnd.Width();
		Height = Wnd.Height();

		Wnd.PollMessages();
		if (!Wnd.IsValid())
			break;

		INT32 NewWidth  = Wnd.Width();
		INT32 NewHeight = Wnd.Height();

		if (Width != NewWidth || Height != NewHeight) {
			D3D.Resize(NewWidth, NewHeight);

			if (!D3D.IsValid()) {
				LOG(Error,
					"Failed to resize buffers.\nHRESULT status: 0x%08x",
					D3D.GetLastResult());

				break;
			}
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::ShowDemoWindow();

		ImGui::Render();

		const FLOAT ClearColor[] = { 
			0.0f,
			0.0f,
			1.0f,
			1.0f
		};

		D3D.Context->OMSetRenderTargets(1, D3D.RenderTarget.GetAddressOf(), NULL);
		D3D.Context->ClearRenderTargetView(D3D.RenderTarget.Get(), ClearColor);

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		D3D.SwapChain->Present(1, 0);
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	Phosdb::Window::Destroy();

	return 0;
}