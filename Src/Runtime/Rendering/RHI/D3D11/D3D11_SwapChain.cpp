#include "D3D11_SwapChain.h"
#include "D3D11_Device.h"
#include "D3D11_Texture.h"
#include <cassert>

namespace Miyadaiku
{
D3D11_SwapChain::D3D11_SwapChain()
{
}

D3D11_SwapChain::~D3D11_SwapChain()
{
}

bool D3D11_SwapChain::Init(void*							  _windowHandle,
						   const std::shared_ptr<RHI_Device>& _device,
						   uint32_t _width, uint32_t _height)
{
	const HWND hWnd = static_cast<HWND>(_windowHandle);
	assert(hWnd != nullptr);

	auto device = static_pointer_cast<D3D11_Device>(_device);

	IDXGIFactory* factory = device->GetGIFactory();
	
	// スワップチェイン作成(フロントバッファに表示可能なバックバッファを持つもの)
	DXGI_SWAP_CHAIN_DESC DXGISwapChainDesc = {};
	DXGISwapChainDesc.BufferDesc.Width = _width;
	DXGISwapChainDesc.BufferDesc.Height = _height;
	DXGISwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	DXGISwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	DXGISwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGISwapChainDesc.BufferDesc.ScanlineOrdering =
		DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	DXGISwapChainDesc.BufferDesc.Scaling =
		DXGI_MODE_SCALING_UNSPECIFIED; // DXGI_MODE_SCALING_CENTERED;	//
									   // DXGI_MODE_SCALING_UNSPECIFIED
	DXGISwapChainDesc.SampleDesc.Count = 1;
	DXGISwapChainDesc.SampleDesc.Quality = 0;
	DXGISwapChainDesc.BufferUsage =
		DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	DXGISwapChainDesc.BufferCount = 2;
	DXGISwapChainDesc.OutputWindow = hWnd;
	DXGISwapChainDesc.Windowed = TRUE;
	DXGISwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	DXGISwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	
    IDXGISwapChain* pSwapChain = nullptr;

	if (FAILED(factory->CreateSwapChain(device->GetDevice(), &DXGISwapChainDesc,
										&pSwapChain)))
	{

		Release();
		return false;
	}
	//factory->Release();
	m_swapChainHandle.m_pData = pSwapChain;

	// スワップチェインからバックバッファ取得
	ID3D11Texture2D* pBackBuffer;
	if (FAILED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
										 (void**)&pBackBuffer)))
	{
		Release();
		return false;
	}

	// バックバッファリソースからビューを作成
	m_spBackBuffer = std::make_shared<DX11_Texture>(_device);
	RHIResourceHandle_Texture backBufferHandle;
	backBufferHandle.m_pData = pBackBuffer;
	if (m_spBackBuffer->Create(backBufferHandle) == false)
	{
		Release();
		return false;
	}

	pBackBuffer->Release();

	return true;
}

void D3D11_SwapChain::Present()
{
	// 第1引数 画面同期の方法
	// 0：特に同期をしない, 1：可変フレームレート対応
	// if (FAILED(m_cpGISwapChain.Get()->Present(0, 0)))
	if (FAILED(reinterpret_cast<IDXGISwapChain*>(m_swapChainHandle.m_pData)
				   ->Present(1, 0)))
	{
		assert(0 && "画面更新の失敗");
	}
}
void D3D11_SwapChain::Release()
{
	IDXGISwapChain* swap_chain = reinterpret_cast<IDXGISwapChain*>(m_swapChainHandle.m_pData);

	if (swap_chain)
	{
		swap_chain->Release();
		m_swapChainHandle.Reset();
	}
	
	if (m_spBackBuffer)
	{
		m_spBackBuffer->Release();
		m_spBackBuffer = nullptr;
	}
	if (m_spDepthStencilBuffer)
	{
		m_spDepthStencilBuffer->Release();
		m_spDepthStencilBuffer = nullptr;
	}
}
} // namespace Miyadaiku