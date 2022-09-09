#pragma once
#include "../Core/Subsystem.h"

namespace Miyadaiku
{
// 前方宣言
class RHI_Device;
class RHI_SwapChain;
class RHI_Shader;

class Renderer final : public Subsystem
{
public:
	~Renderer();

	void OnAwake() override;

	void OnShutdown() override;

	void Present();

	std::shared_ptr<RHI_Device> GetRHIDevice()
	{
		return m_spRHIDevice;
	}

private:
	std::shared_ptr<RHI_Device>	   m_spRHIDevice = nullptr;
	std::shared_ptr<RHI_SwapChain> m_spRHISwapChain = nullptr;

	// TODO: シェーダはどこか（RHI_CommandListとか(RHI_?)RenderingPipelineとか）に移動
	// アイデアとして、RenderingPipelineの概念はUnityに近い感じ。RenderPassの組み合わせとか？
	
	std::shared_ptr<RHI_Shader> m_spRHIVertexShader = nullptr;
	std::shared_ptr<RHI_Shader> m_spRHIPixelShader = nullptr;
};
} // namespace Miyadaiku