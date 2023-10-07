#include "Direct3D.hh"
#include "Window.hh"

namespace Phosdb {
Direct3D::Direct3D(const Window &Wnd) {
	UINT32 Flags = 0;
#if DEBUG
	Flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	DWM_TIMING_INFO TimingInfo = {
		.cbSize = sizeof(DWM_TIMING_INFO)
	};

	if (FAILED(Result = DwmGetCompositionTimingInfo(NULL, &TimingInfo)))
		return;

	DXGI_MODE_DESC BufferDesc = {
		.Width            = (UINT32)Wnd.Width(),
		.Height           = (UINT32)Wnd.Height(),
		.RefreshRate      = {
			.Numerator   = TimingInfo.rateRefresh.uiNumerator,
			.Denominator = TimingInfo.rateRefresh.uiDenominator
		},
		.Format           = DXGI_FORMAT_R8G8B8A8_UNORM,
		.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
		.Scaling          = DXGI_MODE_SCALING_UNSPECIFIED
	};

	DXGI_SWAP_CHAIN_DESC SwapChainDesc = {
		.BufferDesc   = BufferDesc,
		.SampleDesc   = {
			.Count   = 1,
			.Quality = 0
		},
		.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT,
		.BufferCount  = 2,
		.OutputWindow = Wnd.GetNativeHandle(),
		.Windowed     = TRUE,
		.SwapEffect   = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL,
		.Flags        = 0
	};

	Result = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		Flags,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&SwapChainDesc,
		&SwapChain,
		&Device,
		NULL,
		&Context);

	if (FAILED(Result))
		return;

	if (FAILED(Result = SwapChain->GetBuffer(0, IID_PPV_ARGS(&Backbuffer))))
		return;

	Result = Device->CreateRenderTargetView(Backbuffer.Get(), NULL, &RenderTarget);
}

BOOL Direct3D::Resize(
	INT32 Width,
	INT32 Height
) {
	RenderTarget.Reset();
	Backbuffer.Reset();

	if (FAILED(Result = SwapChain->ResizeBuffers(0, Width, Height, DXGI_FORMAT_UNKNOWN, 0)))
		return FALSE;

	if (FAILED(Result = SwapChain->GetBuffer(0, IID_PPV_ARGS(&Backbuffer))))
		return FALSE;

	return SUCCEEDED(Result = Device->CreateRenderTargetView(Backbuffer.Get(), NULL, &RenderTarget));
}
}