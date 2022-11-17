#pragma once
#include "../RHI_ConstantBuffer.h"

namespace Miyadaiku
{
class D3D11_ConstantBuffer : public RHI_ConstantBuffer
{
public:
	D3D11_ConstantBuffer(const std::shared_ptr<RHI_Device>& _spRHIDevice)
		: RHI_ConstantBuffer(_spRHIDevice)
	{
	}

	void* Map() override;
	void Unmap() override;

private:
	bool Internal_Create() override;
};
} // namespace Miyadaiku

