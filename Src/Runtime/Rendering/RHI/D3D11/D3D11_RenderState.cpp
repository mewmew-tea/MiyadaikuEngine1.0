#include "D3D11_RenderState.h"
#include "D3D11Common.h"
#include "D3D11_Device.h"

namespace Miyadaiku
{
D3D11_SamplerState::D3D11_SamplerState(
	const std::shared_ptr<RHI_Device>& _spRHIDevice,
	const SS_FilterMode _filterMode, const SS_AddressMode _addressMode)
	: RHI_SamplerState(_spRHIDevice, _filterMode, _addressMode)
{
	D3D11_SAMPLER_DESC desc = {};
	
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	desc.MipLODBias = 0;
	// desc.MaxAnisotropy = 0.0f;

	desc.BorderColor[0] = desc.BorderColor[1] = desc.BorderColor[2] =
		desc.BorderColor[3] = 0;

	switch (_filterMode)
	{
	case SS_FilterMode::Linear:
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		break;
	case SS_FilterMode::Point:
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		break;
	case SS_FilterMode::Aniso:
		desc.Filter = D3D11_FILTER_ANISOTROPIC;
		// smpDesc.MaxAnisotropy = 4;
		break;
	default:
		break;
	}

	switch (_addressMode)
	{
	case SS_AddressMode::Wrap:
		desc.AddressU = desc.AddressV = desc.AddressW =
			D3D11_TEXTURE_ADDRESS_WRAP;
		break;
	case SS_AddressMode::Mirror:
		desc.AddressU = desc.AddressV = desc.AddressW =
			D3D11_TEXTURE_ADDRESS_MIRROR;
		break;
	case SS_AddressMode::Clamp:
		desc.AddressU = desc.AddressV = desc.AddressW =
			D3D11_TEXTURE_ADDRESS_CLAMP;
		break;
	default:
		break;
	}

	if (FAILED(std::static_pointer_cast<D3D11_Device>(_spRHIDevice)
				   ->GetDevice()
				   ->CreateSamplerState(&desc,
										reinterpret_cast<ID3D11SamplerState**>(
											&m_resourceHandle.m_pData))))
	{
		assert(0 && "Create D3D11SamplerState failed.");
	}
}
D3D11_SamplerState::~D3D11_SamplerState()
{
	// if (m_resourceHandle.m_pData)
	//{
	//	reinterpret_cast<ID3D11SamplerState*>(m_resourceHandle.m_pData)
	//		->Release();
	//	m_resourceHandle.Reset();
	// }
	D3D11_SafeCastRelease<ID3D11SamplerState>(m_resourceHandle.m_pData);
}

//-------------------------------------------------------
D3D11_RasterizerState::D3D11_RasterizerState(
	const std::shared_ptr<RHI_Device>& _spRHIDevice,
	const RS_CullMode _cullMode, const RS_FillMode _fillMode)
	: RHI_RasterizerState(_spRHIDevice, _cullMode, _fillMode)
{
	D3D11_RASTERIZER_DESC desc = {};
	desc.DepthClipEnable = true;

	switch (_cullMode)
	{
	case RS_CullMode::None:
		desc.CullMode = D3D11_CULL_NONE;
		break;
	case RS_CullMode::Front:
		desc.CullMode = D3D11_CULL_FRONT;
		break;
	case RS_CullMode::Back:
		desc.CullMode = D3D11_CULL_BACK;
		break;
	default:
		break;
	}

	switch (_fillMode)
	{
	case RS_FillMode::Solid:
		desc.FillMode = D3D11_FILL_SOLID;
		break;
	case RS_FillMode::Wire:
		desc.FillMode = D3D11_FILL_WIREFRAME;
		break;
	default:
		break;
	}

	ID3D11RasterizerState* state = nullptr;
	
	ID3D11Device* pDevice =
		std::static_pointer_cast<D3D11_Device>(_spRHIDevice)->GetDevice();
	
	auto hr = pDevice->CreateRasterizerState(
		&desc, &state);
	if (FAILED(hr))
	{
		assert(0 && "ラスタライザステート作成失敗");
		return;
	}

	m_resourceHandle.m_pData = state;
}
D3D11_RasterizerState::~D3D11_RasterizerState()
{
	D3D11_SafeCastRelease<ID3D11SamplerState>(m_resourceHandle.m_pData);
}
} // namespace Miyadaiku