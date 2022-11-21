#pragma once
#include "../RHI_Texture.h"
#include "D3D11Common.h"

namespace Miyadaiku
{
class D3D11_Texture : public RHI_Texture
{
public:
	//DX11_Texture() = delete;
	D3D11_Texture(const std::shared_ptr<RHI_Device>& _spRhiDevice)
		: RHI_Texture(_spRhiDevice)
	{
	
	}
	~D3D11_Texture()
	{
		Release();
	}

	bool Create(RHIResourceHandle_Texture& _srcResource) override;

	bool Create(std::string_view _filePath) override;

	bool Create(uint32_t _w, uint32_t _h,
				RHI_FORMAT _format = RHI_FORMAT::R8G8B8A8_UNORM,
				uint32_t _arrayCnt = 1, const void* _pFillData = nullptr,
				const uint32_t _sysMemPitch = 4) override;

	bool CreateDepthStencil(int _w, int _h) override;

	void Release() override;

private:
	bool CreateRTV();

	// テクスチャについての情報
	D3D11_TEXTURE2D_DESC m_desc = {};
};
}