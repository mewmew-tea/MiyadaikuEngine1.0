#pragma once
#include <memory>
#include "../RenderingResource.h"

namespace Miyadaiku
{
class RHI_Device;
class RHI_Texture
{
public:
	//RHI_Texture() = delete;
	RHI_Texture(const std::shared_ptr<RHI_Device>& _spRhiDevice)
	{
		m_wpRHIDevice = _spRhiDevice;
	}
	~RHI_Texture()
	{
	}

	virtual bool Create(RHIResourceHandle_Texture& _srcResource) = 0;

	virtual void Release() = 0;

	RHIResourceHandle_Texture const& GetTextureHandle()
	{
		return m_textureHandle;
	}
	RHIResourceHandle_SRV const& GetSRVHandle()
	{
		return m_srvHandle;
	}
	RHIResourceHandle_RT const& GetRTHandle()
	{
		return m_rtHandle;
	}
	RHIResourceHandle_DS const& GetDSHandle()
	{
		return m_dsHandle;
	}

protected:
	// テクスチャのリソースやビューなど
	RHIResourceHandle_Texture m_textureHandle;
	RHIResourceHandle_SRV	  m_srvHandle;
	RHIResourceHandle_RT	  m_rtHandle;
	RHIResourceHandle_DS	  m_dsHandle;

	std::weak_ptr<RHI_Device> m_wpRHIDevice;
};
} // namespace Miyadaiku