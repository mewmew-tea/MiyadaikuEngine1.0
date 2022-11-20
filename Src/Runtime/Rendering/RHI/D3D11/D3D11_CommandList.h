#pragma once
#include "../RHI_CommandList.h"

namespace Miyadaiku
{
class D3D11_CommandList : public RHI_CommandList
{
public:
	D3D11_CommandList(const std::shared_ptr<RHI_Device>& _spRhiDevice)
		: RHI_CommandList(_spRhiDevice)
	{
	}

	void SetPipelineState(RHI_PipelineState& _pso) override;

	void SetViewport(const RectangleI& _rect, float _minDepth = 0.0f,
					 float _maxDepth = 1.0f) override;

	void SetSamplerState(
		const std::uint32_t						 _slot,
		const std::shared_ptr<RHI_SamplerState>& _spSamplerState) override;

	void SetConstantBuffer(const std::uint32_t _slot,
						   const std::shared_ptr<RHI_ConstantBuffer>
							   _pConstantBuffer) override;

	void SetVertexBuffer(
		const std::uint32_t						_slot,
					const std::shared_ptr<RHI_VertexBuffer> _spVertexBuffer,
					const uint32_t							_offset) override;

	void SetIndexBuffer(const std::uint32_t					   _slot,
						const std::shared_ptr<RHI_IndexBuffer> _spIndexBuffer,
						const uint32_t						   _offset) override;
	
	// Draw
	virtual void Draw(uint32_t _indexCount, uint32_t _vertexStart);
	virtual void DrawIndexed(uint32_t _indexCount, uint32_t _indexOffset,
							 uint32_t _vertexOffset = 0);
};
} // namespace Miyadaiku
