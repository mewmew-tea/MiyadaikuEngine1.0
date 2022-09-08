#include "D3D11_Texture.h"
#include "D3D11_Device.h"

#include "../../../Core/Engine.h"
#include "../../Renderer.h"

#pragma warning(push)
#pragma warning(disable : 6262)
#pragma warning(disable : 26451)
#define STB_IMAGE_IMPLEMENTATION
#include "../../../External/stb/stb_image.h"
#pragma warning(pop)

namespace Miyadaiku
{

static bool D3D11_CreateRTV(ID3D11Texture2D*		   _resource,
							ID3D11RenderTargetView**   _ppRTV,
							ID3D11Device*			   _pDevice)
{
	if (_resource == nullptr)
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC desc;
	_resource->GetDesc(&desc);

	// BindFlagsチェック
	if (_ppRTV && (desc.BindFlags & D3D11_BIND_RENDER_TARGET))
	{
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = desc.Format;

		if (desc.ArraySize == 1)
		{
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		}
		// Texture2DArray
		else
		{
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			rtvDesc.Texture2DArray.ArraySize = desc.ArraySize;
			rtvDesc.Texture2DArray.FirstArraySlice = 0;
			rtvDesc.Texture2DArray.MipSlice = 0;
		}

		if (FAILED(
				_pDevice->CreateRenderTargetView(_resource, &rtvDesc, _ppRTV)))
		{
			assert(0 && "D3D11_RenderTargetView作成失敗");
			return false;
		}
	}

	return true;
}

static bool D3D11_CreateSRV(ID3D11Texture2D*		   _resource,
							ID3D11ShaderResourceView** _ppSRV,
							ID3D11Device*			   _pDevice)
{
	if (_resource == nullptr)
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC desc;
	_resource->GetDesc(&desc);

	// BindFlagsチェック
	if (_ppSRV && (desc.BindFlags & D3D11_BIND_SHADER_RESOURCE))
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		if (desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
		{
			switch (desc.Format)
			{
				// 16ビット
			case DXGI_FORMAT_R16_TYPELESS:
				srvDesc.Format = DXGI_FORMAT_R16_UNORM;
				break;
				// 32ビット
			case DXGI_FORMAT_R32_TYPELESS:
				srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
				break;
				// 24ビット(Zバッファ) + 8ビット(ステンシルバッファ)
			case DXGI_FORMAT_R24G8_TYPELESS:
				srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
				break;
			default:
				assert(0 && "D3D11_SHADER_RESOURCE 対応していないフォーマット");
				break;
			}
		}
		else
		{
			srvDesc.Format = desc.Format;
		}

		if (desc.ArraySize == 1)
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.MipLevels = desc.MipLevels;
			if (srvDesc.Texture2D.MipLevels <= 0)
				srvDesc.Texture2D.MipLevels = 1;
		}
		// Texture2DArrayのとき
		else
		{
			// キューブマップ
			if (desc.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE)
			{
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			}
			// 通常テクスチャ配列
			else
			{
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
			}
			srvDesc.Texture2DArray.MostDetailedMip = 0;
			srvDesc.Texture2DArray.MipLevels = desc.MipLevels;
			srvDesc.Texture2DArray.ArraySize = desc.ArraySize; // 要素数
			srvDesc.Texture2DArray.FirstArraySlice = 0;
		}

		if (FAILED(_pDevice->CreateShaderResourceView(_resource, &srvDesc,
													  _ppSRV)))
		{
			assert(0 && "D3D11_ShaderResourceView作成失敗");
			return false;
		}
	}

	return true;
}

static bool D3D11_CreateDSV(ID3D11Texture2D*		   _resource,
							ID3D11DepthStencilView**   _ppDSV,
							ID3D11Device*			   _pDevice)
{
	if (_resource == nullptr)
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC desc;
	_resource->GetDesc(&desc);

	// BindFlagsチェック
	if (_ppDSV && (desc.BindFlags & D3D11_BIND_DEPTH_STENCIL))
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		switch (desc.Format)
		{
			// 16ビット
		case DXGI_FORMAT_R16_TYPELESS:
			dsvDesc.Format = DXGI_FORMAT_D16_UNORM;
			break;
			// 32ビット
		case DXGI_FORMAT_R32_TYPELESS:
			dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
			break;
			// 24ビット(Zバッファ) + 8ビット(ステンシルバッファ)
		case DXGI_FORMAT_R24G8_TYPELESS:
			dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			break;
		default:
			assert(0 && "[DepthStencil] 対応していないフォーマットです");
			break;
		}


		if (desc.ArraySize == 1)
		{
			dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			dsvDesc.Texture2D.MipSlice = 0;
		}
		// Texture2DArrayのとき
		else
		{
			dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
			dsvDesc.Texture2DArray.ArraySize = desc.ArraySize;
			dsvDesc.Texture2DArray.MipSlice = 0;
			dsvDesc.Texture2DArray.FirstArraySlice = 0;
		}

		if (FAILED(
				_pDevice->CreateDepthStencilView(_resource, &dsvDesc, _ppDSV)))
		{
			assert(0 && "DepthStencilViewの作成に失敗");
			return false;
		}
	}

	return true;
}

static bool D3D11_CreateViewsFromTexture2D(ID3D11Texture2D*			  _resource,
										   ID3D11ShaderResourceView** _ppSRV,
										   ID3D11RenderTargetView**	  _ppRTV,
										   ID3D11DepthStencilView**	  _ppDSV,
										   ID3D11Device*			  _pDevice)
{
	if (_resource == nullptr)
	{
		return false;
	}

	// RenderTargetView作成
	if (!D3D11_CreateRTV(_resource, _ppRTV, _pDevice))
	{
		return false;
	}

	// ShaderResourceView作成
	if (!D3D11_CreateSRV(_resource, _ppSRV, _pDevice))
	{
		return false;
	}

	// DepthStencilView作成
	if (!D3D11_CreateDSV(_resource, _ppDSV, _pDevice))
	{
		return false;
	}

	return true;
}

//================================================================================

bool D3D11_Texture::Create(RHIResourceHandle_Texture& _srcResource)
{
	ID3D11Texture2D* pSrcTex =
		reinterpret_cast<ID3D11Texture2D*>(_srcResource.m_pData);

	if (!pSrcTex)
	{
		return false;
	}
	// 必要な情報をコピーしておいたのちに、RTV生成
	m_textureHandle.m_pData = pSrcTex;
	pSrcTex->GetDesc(&m_desc);
	return CreateRTV();
}

bool D3D11_Texture::Create(std::string_view _filePath)
{
	std::shared_ptr<D3D11_Device> spDevice =
		std::static_pointer_cast<D3D11_Device>(m_wpRHIDevice.lock());

	// ロード
	int			   width;
	int			   height;
	int			   channels;
	//unsigned char* pImageData;
	 unsigned char* pImageData =
		stbi_load(_filePath.data(), &width, &height, &channels,
				  4); // RGBAの4チャンネル
	if (pImageData)
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA subresourceData = {};
	subresourceData.pSysMem = pImageData;
	subresourceData.SysMemPitch = 4 * width;

	ID3D11Texture2D* texture2D;
	if (FAILED(spDevice->GetDevice()->CreateTexture2D(&desc, &subresourceData,
													  &texture2D)))
	{
		assert(0 && "ID3D11Texture2Dテクスチャ作成失敗");
	}

	free(pImageData);

	if (!D3D11_CreateViewsFromTexture2D(
		texture2D,
		reinterpret_cast<ID3D11ShaderResourceView**>(m_srvHandle.GetAddress()),
		reinterpret_cast<ID3D11RenderTargetView**>(m_rtHandle.GetAddress()),
		reinterpret_cast<ID3D11DepthStencilView**>(m_dsHandle.GetAddress()),
		spDevice->GetDevice()))
	{
		return false;
	}


	return true;
}

void D3D11_Texture::Release()
{
	// if (m_textureHandle.m_pData)
	//{
	//	reinterpret_cast<ID3D11Texture2D*>(m_textureHandle.m_pData)->Release();
	//	m_textureHandle.Reset();
	// }
	if (m_srvHandle.m_pData)
	{
		reinterpret_cast<ID3D11ShaderResourceView*>(m_srvHandle.m_pData)
			->Release();
		m_srvHandle.Reset();
	}
	if (m_rtHandle.m_pData)
	{
		reinterpret_cast<ID3D11RenderTargetView*>(m_rtHandle.m_pData)
			->Release();
		m_rtHandle.Reset();
	}
	if (m_dsHandle.m_pData)
	{
		reinterpret_cast<ID3D11DepthStencilView*>(m_dsHandle.m_pData)
			->Release();
		m_dsHandle.Reset();
	}
}

bool D3D11_Texture::CreateRTV()
{
	if (!m_textureHandle.m_pData)
	{
		return false;
	}

	// 生成
	CD3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = m_desc.Format;
	rtvDesc.ViewDimension =
		D3D11_RTV_DIMENSION_TEXTURE2D; // 生成するテクスチャの種類

	// m_wpRHIDevice =
	//	GetEngine()->GetSubsystemLocator().Get<Renderer>()->GetRHIDevice();
	std::shared_ptr<D3D11_Device> rhiDevice =
		std::static_pointer_cast<D3D11_Device>(m_wpRHIDevice.lock());

	ID3D11Texture2D* pTexData =
		reinterpret_cast<ID3D11Texture2D*>(m_textureHandle.m_pData);
	ID3D11RenderTargetView* pRTV;

	HRESULT hr = rhiDevice->GetDevice()->CreateRenderTargetView(
		pTexData, &rtvDesc, &pRTV);
	if (FAILED(hr))
	{
		assert(0 && "RenderTargetView作成失敗");
		return false;
	}

	m_rtHandle.m_pData = pRTV;
	return true;
}

} // namespace Miyadaiku