#pragma once
#include <cstdint>
#include <memory>

namespace Miyadaiku
{
class RHI_Device;

class RHI_ConstantBuffer
{
public:
	RHI_ConstantBuffer(const std::shared_ptr<RHI_Device>& _spRHIDevice);
	~RHI_ConstantBuffer()
	{
		Release();
	}
	
	template <typename DataType> 
	bool Create()
	{
		m_stride = static_cast<uint32_t>(sizeof(DataType));

		return Internal_Create();
	}

	template <typename DataType> 
	void Write(DataType& _data)
	{
		void* buf = Map();

		if (buf)
		{
			memcpy_s(buf, m_stride, &_data, m_stride);
			Unmap();
		}
	}

	virtual void* Map() = 0;

	virtual void Unmap() = 0;

	void Release()
	{
		Internal_Release();
	}

	//DataType& GetData()
	//{
	//	m_isDirty = true;
	//	return m_data;
	//}

	void* GetResource()
	{
		return m_pResource;
	}

protected:
	virtual bool Internal_Create() = 0;
	virtual void Internal_Release(){};

	//// constant buffer data
	//DataType m_data = {};

	// graphics api resource
	void*					  m_pResource = nullptr;
	void*					  m_pMappedData = nullptr;
	uint32_t				  m_stride = 0;
	bool					  m_isDirty = false;

	std::weak_ptr<RHI_Device> m_wpRHIDevice;
};
} // namespace Miyadaiku