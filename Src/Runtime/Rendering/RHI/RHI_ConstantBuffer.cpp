#include "RHI_ConstantBuffer.h"

namespace Miyadaiku
{

RHI_ConstantBuffer::RHI_ConstantBuffer(
	const std::shared_ptr<RHI_Device>& _spRHIDevice)
{
	m_wpRHIDevice = _spRHIDevice;
}

}