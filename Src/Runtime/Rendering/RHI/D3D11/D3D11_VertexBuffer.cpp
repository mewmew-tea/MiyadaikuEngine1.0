﻿#include "D3D11_VertexBuffer.h"
#include "D3D11_Device.h"
#include <cassert>

namespace Miyadaiku
{
void* D3D11_VertexBuffer::Map()
{
	std::shared_ptr<D3D11_Device> spRHIDevice =
		std::static_pointer_cast<D3D11_Device>(this->m_wpRHIDevice.lock());

	D3D11_MAPPED_SUBRESOURCE subRes;
	const auto				 result = spRHIDevice->GetDeviceContext()->Map(
					  static_cast<ID3D11Buffer*>(this->m_pResource), 0,
					  D3D11_MAP_WRITE_DISCARD, 0, &subRes);
	if (FAILED(result))
	{
		assert(0 && "Failed to map constant buffer.");
		return nullptr;
	}

	return subRes.pData;
}
void D3D11_VertexBuffer::Unmap()
{
	std::shared_ptr<D3D11_Device> spRHIDevice =
		std::static_pointer_cast<D3D11_Device>(this->m_wpRHIDevice.lock());
	spRHIDevice->GetDeviceContext()->Unmap(
		static_cast<ID3D11Buffer*>(this->m_pResource), 0);
}
void D3D11_VertexBuffer::Internal_Release()
{
	static_cast<ID3D11Buffer*>(m_pResource)->Release();
}

bool D3D11_VertexBuffer::Internal_Create(uint32_t	 _bufferSize,
										 const void* _pInitData)
{
	std::shared_ptr<D3D11_Device> spRHIDevice =
		std::static_pointer_cast<D3D11_Device>(this->m_wpRHIDevice.lock());

	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = m_stride * _bufferSize;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = _pInitData;

	HRESULT hr = spRHIDevice->GetDevice()->CreateBuffer(
		&desc, _pInitData ? &data : nullptr,
		reinterpret_cast<ID3D11Buffer**>(&m_pResource));

	if (FAILED(hr))
	{
		return false;
	}

	return true;
}
} // namespace Miyadaiku