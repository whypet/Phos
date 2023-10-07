#pragma once

#include "Phosdb.hh"

using Microsoft::WRL::ComPtr;

namespace Phosdb {
class Window;
class Direct3D {
private:
	HRESULT Result = S_OK;
public:
	ComPtr<IDXGISwapChain>         SwapChain;
	ComPtr<ID3D11Device>           Device;
	ComPtr<ID3D11DeviceContext>    Context;
	ComPtr<ID3D11Texture2D>        Backbuffer;
	ComPtr<ID3D11RenderTargetView> RenderTarget;

	Direct3D(const Window &Wnd);

	inline BOOL IsValid() const {
		return SwapChain != NULL && Device != NULL && Context != NULL && Backbuffer != NULL && RenderTarget != NULL;
	}

	inline HRESULT GetLastResult() const {
		return Result;
	}

	BOOL Resize(
		INT32 Width,
		INT32 Height
	);
};
}