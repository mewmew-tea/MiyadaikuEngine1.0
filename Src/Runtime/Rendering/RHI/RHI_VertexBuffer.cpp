﻿#include "RHI_VertexBuffer.h"


namespace Miyadaiku
{
RHI_VertexBuffer::RHI_VertexBuffer(
	const std::shared_ptr<RHI_Device>& _spRHIDevice)
	: m_wpRHIDevice(_spRHIDevice)
{
}
} // namespace Miyadaiku