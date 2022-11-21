#include "RHI_IndexBuffer.h"


namespace Miyadaiku
{
RHI_IndexBuffer::RHI_IndexBuffer(
	const std::shared_ptr<RHI_Device>& _spRHIDevice)
	: m_wpRHIDevice(_spRHIDevice)
{
}
RHI_IndexBuffer::~RHI_IndexBuffer()
{
	this->Release();
}
} // namespace Miyadaiku