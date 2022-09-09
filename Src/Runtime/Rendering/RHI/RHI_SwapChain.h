#pragma once
#include "../RenderingResource.h"
#include "RHI_Texture.h"
#include <memory>

namespace Miyadaiku
{
class RHI_Device;
class RHI_SwapChain
{
public:
	RHI_SwapChain()
	{
	}
	~RHI_SwapChain()
	{
	}

	virtual bool Init(void*								 _windowHandle,
					  const std::shared_ptr<RHI_Device>& _device, //
					  uint32_t _width, uint32_t _height) = 0;
	virtual void Present() = 0;
	virtual void Release() = 0;

	std::shared_ptr<RHI_Texture> GetBackBuffer()
	{
		return m_spBackBuffer;
	}
	std::shared_ptr<RHI_Texture> GetDepthStencilBuffer()
	{
		return m_spDepthStencilBuffer;
	}

protected:
	RHIResourceHandle_SwapChain	 m_swapChainHandle;	// 例：DXGISwapChain

	std::shared_ptr<RHI_Texture> m_spBackBuffer;
	std::shared_ptr<RHI_Texture> m_spDepthStencilBuffer;
};
} // namespace Miyadaiku