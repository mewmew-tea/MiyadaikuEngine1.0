#pragma once
#include "D3D11Common.h"

#include "../RHI_Device.h"

namespace Miyadaiku
{
class D3D11_Device : public RHI_Device
{
public:
	~D3D11_Device();

	void Init() override;
	void Release() override;

	ID3D11Device* GetDevice()
	{
		return m_pDevice;
	}
	ID3D11DeviceContext* GetDeviceContext()
	{
		return m_pDeviceContext;
	}

	IDXGIFactory* GetGIFactory()
	{
		return m_pFactory;
	}

private:
	ID3D11Device*		 m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
	IDXGIFactory*		 m_pFactory = nullptr;

	ID3D11Debug* m_pD3DDebug = nullptr;
};
} // namespace Miyadaiku