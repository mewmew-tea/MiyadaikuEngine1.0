#include "D3D11_CommandList.h"
#include "../RHI_PipelineState.h"
#include "../RHI_RenderState.h"
#include "../../RenderingResource.h"
#include "D3D11Common.h"
#include "D3D11_Device.h"
#include "D3D11_Shader.h"
#include "D3D11_Texture.h"

namespace Miyadaiku
{
void D3D11_CommandList::SetPipelineState(RHI_PipelineState& _pso)
{
	ID3D11DeviceContext*		  pDeviceContext = nullptr;
	std::shared_ptr<D3D11_Device> spRHIDevice =
		std::static_pointer_cast<D3D11_Device>(m_wpRHIDevice.lock());

	pDeviceContext = spRHIDevice->GetDeviceContext();

	// TODO;InputLayout
	//_pso.

	// Shaders
	// Vertex
	if (_pso.m_spVertexShader)
	{
		ID3D11VertexShader* vs = reinterpret_cast<ID3D11VertexShader*>(
			_pso.m_spVertexShader->GetDataHandle().m_pData);

		ID3D11VertexShader*	 currentVs = nullptr;
		UINT				 instanceCount = 256;
		ID3D11ClassInstance* instances[256];

		pDeviceContext->VSGetShader(&currentVs, instances, &instanceCount);

		// 同一のものがセットされていないときのみ、セットする
		if (vs != currentVs)
		{
			pDeviceContext->VSSetShader(vs, nullptr, 0);
		}
	}
	// Pixel
	if (_pso.m_spVertexShader)
	{
		ID3D11PixelShader* vs = reinterpret_cast<ID3D11PixelShader*>(
			_pso.m_spPixelShader->GetDataHandle().m_pData);

		ID3D11PixelShader*	 currentPs = nullptr;
		UINT				 instanceCount = 256;
		ID3D11ClassInstance* instances[256];

		pDeviceContext->PSGetShader(&currentPs, instances, &instanceCount);

		// 同一のものがセットされていないときのみ、セットする
		if (vs != currentPs)
		{
			pDeviceContext->PSSetShader(vs, nullptr, 0);
		}
	}
	// TODO:ComputeShader

	// BlendState
	if (_pso.m_spBlendState)
	{
		ID3D11BlendState* blendState = reinterpret_cast<ID3D11BlendState*>(
			_pso.m_spBlendState->GetResourceHandle().m_pData);

		// ID3D11PixelShader*	 currentState = nullptr;

		// ID3D11BlendState* currentState =
		//	pDeviceContext->OMGetBlendState(&currentState, );
		const float blendFactor = 0.0f;
		float		blendFactors[4] = {blendFactor, blendFactor, blendFactor,
								   blendFactor};
		pDeviceContext->OMSetBlendState(blendState, blendFactors, 0);
	}

	// DepthStencilState
	if (_pso.m_spDepthStencilState)
	{
		ID3D11DepthStencilState* depthStencilState =
			reinterpret_cast<ID3D11DepthStencilState*>(
				_pso.m_spBlendState->GetResourceHandle().m_pData);
		pDeviceContext->OMSetDepthStencilState(depthStencilState,1);
	}

	// PremitiveTopology
	{
		switch (_pso.m_primitiveTopology)
		{
		case RHI_PrimitiveTopology::TriangleList:
			pDeviceContext->IASetPrimitiveTopology(
				D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			break;
		case RHI_PrimitiveTopology::LineList:
			pDeviceContext->IASetPrimitiveTopology(
				D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		default:
			break;
		}
	}

	// RenderTarget
	{
		// DepthStencil
		ID3D11DepthStencilView* dsv =
			reinterpret_cast<ID3D11DepthStencilView*>(
				_pso.m_spDepthStencilBuffer->GetDSHandle().m_pData);

		// Other
		// Swapchain
		//IDXGISwapChain* pSwapChain = 

		// Textures
		// TODO: multi rendertargets
		ID3D11RenderTargetView* rtvs[] = {
			reinterpret_cast<ID3D11RenderTargetView*>(
				_pso.m_spRenderTargets[0]
				->GetSRVHandle().m_pData)};
		pDeviceContext->OMSetRenderTargets(1, rtvs, dsv);

	}

	// ViewPort
	SetViewport(_pso.m_viewportRect);

	// ClearRendertargets
	// DepthStencil
	{
		pDeviceContext->ClearDepthStencilView(reinterpret_cast<ID3D11DepthStencilView*>(_pso.m_spDepthStencilBuffer->GetDSHandle().m_pData),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	}
	
	// TODO: Muti rendertargets
	{
		ID3D11RenderTargetView* rtv = reinterpret_cast<ID3D11RenderTargetView*>(
			_pso.m_spRenderTargets[0]->GetSRVHandle().m_pData);
		pDeviceContext->ClearRenderTargetView(
			rtv, &_pso.m_spRenderTargetClearColors[0].w);
	}
	

	// 
}
void D3D11_CommandList::SetViewport(const RectangleI& _rect, float _minDepth,
									float _maxDepth)
{
	std::shared_ptr<D3D11_Device> spDevice =
		std::static_pointer_cast<D3D11_Device>(m_wpRHIDevice.lock());

	D3D11_VIEWPORT vp = {};
	vp.TopLeftX = (float)_rect.left;
	vp.TopLeftY = (float)_rect.top;
	vp.Width = (float)_rect.width;
	vp.Height = (float)_rect.height;
	vp.MinDepth = _minDepth;
	vp.MaxDepth = _maxDepth;

	spDevice->GetDeviceContext()->RSSetViewports(1, &vp);
}
void D3D11_CommandList::SetSamplerState(
	const std::uint32_t						 _slot,
	const std::shared_ptr<RHI_SamplerState>& _spSamplerState)
{
	std::shared_ptr<D3D11_Device> spDevice =
		std::static_pointer_cast<D3D11_Device>(m_wpRHIDevice.lock());

	ID3D11DeviceContext* pDeviceContext = spDevice->GetDeviceContext();

	pDeviceContext->PSSetSamplers(_slot, 1,
								  reinterpret_cast<ID3D11SamplerState**>(
									  &_spSamplerState->GetResourceHandle()));

	pDeviceContext->VSSetSamplers(_slot, 1,
								  reinterpret_cast<ID3D11SamplerState**>(
									  &_spSamplerState->GetResourceHandle()));
}
void D3D11_CommandList::Draw(uint32_t _indexCount, uint32_t _vertexStart)
{
	std::static_pointer_cast<D3D11_Device>(m_wpRHIDevice.lock())
		->GetDeviceContext()
		->Draw(static_cast<UINT>(_indexCount), static_cast<UINT>(_vertexStart));
}
void D3D11_CommandList::DrawIndexed(uint32_t _indexCount, uint32_t _indexOffset,
									uint32_t _vertexOffset)
{
	std::static_pointer_cast<D3D11_Device>(m_wpRHIDevice.lock())
		->GetDeviceContext()
		->DrawIndexed(static_cast<UINT>(_indexCount),
					  static_cast<UINT>(_indexOffset),
					  static_cast<INT>(_vertexOffset));
}
} // namespace Miyadaiku