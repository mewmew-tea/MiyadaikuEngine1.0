#pragma once
#include "../../Core/Math/RectangleI.h"
#include "../../Core/Math/Vector4.h"
#include <array>
#include <memory>

namespace Miyadaiku
{
struct RHI_SamplerState;
struct RHI_BlendState;
struct RHI_RasterizerState;
struct RHI_DepthStencilState;

class RHI_SwapChain;
class RHI_Shader;
class RHI_Texture;
class RHI_Device;

static const uint8_t RHI_RENDER_TARGET_MAX = 8;

// TODO: InputLayoutもここで管理する
class RHI_PipelineState
{
public:
	// Shaders
	std::shared_ptr<RHI_Shader> m_spVertexShader;
	std::shared_ptr<RHI_Shader> m_spPixelShader;
	// std::shared_ptr<RHI_Shader> m_spComputeShader;

	// Render States
	std::shared_ptr<RHI_RasterizerState>   m_spRasterizerState;
	std::shared_ptr<RHI_BlendState>		   m_spBlendState;
	std::shared_ptr<RHI_DepthStencilState> m_spDepthStencilState;

	RHI_PrimitiveTopology m_primitiveTopology = RHI_PrimitiveTopology::TriangleList;

	RectangleI m_viewportRect;


	// レンダーターゲット
	std::shared_ptr<RHI_Texture> m_spDepthStencilBuffer;
	std::array<std::shared_ptr<RHI_Texture>, RHI_RENDER_TARGET_MAX>
		m_spRenderTargets;
	//std::shared_ptr<RHI_SwapChain> m_spSwapChain;
	std::array<Vector4, RHI_RENDER_TARGET_MAX>
		m_spRenderTargetClearColors;

private:
	std::weak_ptr<RHI_Device> m_upRHIDevice;
};
} // namespace Miyadaiku