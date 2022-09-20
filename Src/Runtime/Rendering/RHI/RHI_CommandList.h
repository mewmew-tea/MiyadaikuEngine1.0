﻿#pragma once
#include "../../Core/Math/RectangleI.h"
#include "../RenderingResource.h"
#include "RHI_RenderState.h"
#include "RHI_PipelineState.h"
#include <memory>

namespace Miyadaiku
{
class RHI_Device;
class RHI_PipelineState;

/**
 * CommandList（D3D11ではDeviceContextと同等の役割をします）。
 * ViewPortのセットなどもここで行う。
 */
class RHI_CommandList
{
public:
	RHI_CommandList(const std::shared_ptr<RHI_Device>& _spRhiDevice)
	{
		m_wpRHIDevice = _spRhiDevice;
	}

	virtual void SetPipelineState(RHI_PipelineState& _pso) = 0;

	virtual void SetViewport(const RectangleI& _rect, float _minDepth = 0.0f,
							 float _maxDepth = 1.0f) = 0;

	virtual void SetSamplerState(
		const std::uint32_t						 _slot,
		const std::shared_ptr<RHI_SamplerState>& _spSamplerState) = 0;

	// Draw
	virtual void Draw(uint32_t _indexCount, uint32_t _vertexStart) = 0;
	virtual void DrawIndexed(uint32_t _indexCount, uint32_t _indexOffset,
							 uint32_t _vertexOffset = 0) = 0;


	// Vertex


protected:
	std::weak_ptr<RHI_Device> m_wpRHIDevice;

	
    // Pipelinesa
	RHI_PipelineState m_pso;
};
} // namespace Miyadaiku