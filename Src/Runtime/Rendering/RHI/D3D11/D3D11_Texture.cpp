#include "D3D11_Texture.h"

#include "../../../Core/Engine.h"
#include "../../Renderer.h"

#include "D3D11_Device.h"

namespace Miyadaiku
{
bool DX11_Texture::Create(RHIResourceHandle_Texture& _srcResource)
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

void DX11_Texture::Release()
{
	//if (m_textureHandle.m_pData)
	//{
	//	reinterpret_cast<ID3D11Texture2D*>(m_textureHandle.m_pData)->Release();
	//	m_textureHandle.Reset();
	//}
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

bool DX11_Texture::CreateRTV()
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

	//m_wpRHIDevice =
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