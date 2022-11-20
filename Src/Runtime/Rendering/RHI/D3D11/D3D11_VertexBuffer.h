﻿#pragma once
#include "../RHI_VertexBuffer.h"


namespace Miyadaiku
{
class D3D11_VertexBuffer : public RHI_VertexBuffer
{
public:
	D3D11_VertexBuffer(const std::shared_ptr<RHI_Device>& _spRHIDevice)
		: RHI_VertexBuffer(_spRHIDevice)
	{
	}

	void* Map() override;
	void  Unmap() override;

private:
	bool Internal_Create(uint32_t _bufferSize, const void* _pInitData) override;
};
}