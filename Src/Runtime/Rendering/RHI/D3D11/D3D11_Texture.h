#pragma once
#include "../RHI_Texture.h"
#include "D3D11Common.h"

namespace Miyadaiku
{
class DX11_Texture : public RHI_Texture
{
public:
	//DX11_Texture() = delete;
	DX11_Texture(const std::shared_ptr<RHI_Device>& _spRhiDevice)
		: RHI_Texture(_spRhiDevice)
	{
	
	}
	~DX11_Texture()
	{
		Release();
	}

	bool Create(RHIResourceHandle_Texture& _srcResource) override;

	void Release() override;

private:
	bool CreateRTV();

	// テクスチャについての情報
	D3D11_TEXTURE2D_DESC m_desc = {};
};
}