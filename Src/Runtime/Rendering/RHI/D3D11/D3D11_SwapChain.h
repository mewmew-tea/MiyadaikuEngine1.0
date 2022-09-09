#pragma once
#include "../RHI_SwapChain.h"

namespace Miyadaiku
{
class D3D11_SwapChain : public RHI_SwapChain
{
public:
	D3D11_SwapChain();
	~D3D11_SwapChain();

	bool Init(void*								 _windowHandle,
			  const std::shared_ptr<RHI_Device>& _device, //
			  uint32_t _width, uint32_t _height) override;
	void Present() override;
	void Release() override;

private:
};
} // namespace Miyadaiku
