#pragma once
#include "../Core/Subsystem.h"

namespace Miyadaiku
{
// 前方宣言
class RHI_Device;
class RHI_SwapChain;

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
	std::shared_ptr<RHI_Device> m_spRHIDevice  = nullptr;
	std::shared_ptr<RHI_SwapChain> m_spRHISwapChain = nullptr;
};
} // namespace Miyadaiku