#pragma once
#include "../Core/Subsystem.h"
#include "./CommonConstantBuffer.h"

namespace Miyadaiku
{
// 前方宣言
class RHI_Device;
class RHI_SwapChain;
class RHI_Shader;
class RHI_CommandList;

struct RHI_RasterizerState;
struct RHI_SamplerState;

class RHI_ConstantBuffer;

class Renderer final : public Subsystem
{
public:
	~Renderer();

	void OnAwake() override;

	void OnShutdown() override;

	void Tick();

	void Present();

	std::shared_ptr<RHI_Device> GetRHIDevice()
	{
		return m_spRHIDevice;
	}

private:
	std::shared_ptr<RHI_Device>		 m_spRHIDevice = nullptr;
	std::shared_ptr<RHI_SwapChain>	 m_spRHISwapChain = nullptr;
	std::shared_ptr<RHI_CommandList> m_spRHIComandList = nullptr;

	// TODO:
	// シェーダはどこか（RHI_CommandListとか(RHI_?)RenderingPipelineとか）に移動
	// アイデアとして、RenderingPipelineの概念はUnityに近い感じ。RenderPassの組み合わせとか？

	std::shared_ptr<RHI_Shader> m_spRHIVertexShader = nullptr;
	std::shared_ptr<RHI_Shader> m_spRHIPixelShader = nullptr;


	std::shared_ptr<RHI_RasterizerState> m_spRHIRasterizerState = nullptr;
	std::shared_ptr<RHI_SamplerState> m_spRHISamplerState = nullptr;

	// Constant buffers
	// TODO: class to combine data and CBs
	Cb_Camera							m_cbCameraData;
	std::shared_ptr<RHI_ConstantBuffer> m_spCbCamera = nullptr;
};
} // namespace Miyadaiku