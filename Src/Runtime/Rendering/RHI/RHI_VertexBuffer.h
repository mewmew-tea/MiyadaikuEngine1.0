#pragma once

#include <cstdint>
#include <memory>
#include <vector>

namespace Miyadaiku
{
class RHI_Device;
class RHI_VertexBuffer
{
public:
	RHI_VertexBuffer(const std::shared_ptr<RHI_Device>& _spRHIDevice);
	virtual ~RHI_VertexBuffer()
	{
		Release();
	}
	template <typename DataType>
	bool Create(uint32_t _bufferSize, const void* _pInitData = nullptr)
	{
		m_stride = sizeof(DataType);

		return Internal_Create(_bufferSize, _pInitData);
	}

	void Write(const void* _pSrcData, uint32_t _size)
	{
		void* buf = Map();

		if (buf)
		{
			memcpy_s(buf, m_stride, _pSrcData, _size);
			Unmap();
		}
	}

	virtual void* Map() = 0;

	virtual void Unmap() = 0;

	void Release()
	{
		Internal_Release();
	}

	void* GetResource()
	{
		return m_pResource;
	}

	uint32_t GetStride()
	{
		return m_stride;
	}

protected:
	virtual bool			  Internal_Create(uint32_t	  _bufferSize,
											  const void* _pInitData) = 0;
	virtual void			  Internal_Release(){};
	// graphics api resource
	void*					  m_pResource = nullptr;
	void*					  m_pMappedData = nullptr;
	uint32_t				  m_stride = 0;

	std::weak_ptr<RHI_Device> m_wpRHIDevice;
};
} // namespace Miyadaiku