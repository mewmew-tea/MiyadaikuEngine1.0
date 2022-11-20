#pragma once
#include "../RHI_IndexBuffer.h"


namespace Miyadaiku
{
class D3D11_IndexBuffer : public RHI_IndexBuffer
{
public:
	D3D11_IndexBuffer(const std::shared_ptr<RHI_Device>& _spRHIDevice)
		: RHI_IndexBuffer(_spRHIDevice)
	{
	}

	void* Map() override;
	void  Unmap() override;

private:
	bool Internal_Create(uint32_t _bufferSize, const void* _pInitData) override;
};
}