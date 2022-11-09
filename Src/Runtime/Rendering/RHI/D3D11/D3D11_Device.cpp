#include "D3D11_Device.h"
#include <cassert>
#include <cstdio>

namespace Miyadaiku
{
D3D11_Device::~D3D11_Device()
{
	Release();
}
void D3D11_Device::Init()
{
	HRESULT		  hr;
	IDXGIFactory* factory = nullptr;

	hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1),
							reinterpret_cast<void**>(&factory));
	if (FAILED(hr))
	{
		assert(0 && "DXGIFactory作成失敗");
	}
	if (factory == nullptr)
	{
		assert(0 && "DXGIFactory作成失敗");
	}
	m_pFactory = factory;

	// DeviceとDeviceContext作成

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1, // Direct3D 11.1  ShaderModel 5
	};
	D3D_FEATURE_LEVEL futureLevel;
	// デバッグ情報取得
	UINT			  creationFlags = 0;
	// if ()
#ifdef _DEBUG
	{
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	}
#endif // _DEBUG

	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
								 creationFlags, featureLevels,
								 _countof(featureLevels), D3D11_SDK_VERSION,
								 &m_pDevice, &futureLevel, &m_pDeviceContext)))
	{
		assert(0 && "D3DDevice作成失敗");
	}
#ifdef _DEBUG
	hr = m_pDevice->QueryInterface(__uuidof(ID3D11Debug),
								   reinterpret_cast<void**>(&m_pD3DDebug));
	// 詳細表示
	hr = m_pD3DDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif // DEBUG
}
void D3D11_Device::Release()
{
	if (m_pFactory)
	{
		m_pFactory->Release();
		m_pFactory = nullptr;
	}
	if (m_pDeviceContext)
	{
		m_pDeviceContext->Release();
		m_pDeviceContext = nullptr;
	}
	if (m_pDevice)
	{
		m_pDevice->Release();
		m_pDevice = nullptr;
	}
}
} // namespace Miyadaiku