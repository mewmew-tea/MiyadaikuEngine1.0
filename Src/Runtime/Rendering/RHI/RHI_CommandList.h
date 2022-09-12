#pragma once
#include "../../Core/Math/RectangleI.h"
#include <memory>

namespace Miyadaiku
{
class RHI_Device;

/**
 * CommandList（D3D11ではDeviceContextと同等の役割をします）。
 * ViewPortのセットなどもここで行う。
 */
class RHI_CommandList
{
public:
	RHI_CommandList(const std::shared_ptr<RHI_Device>& _spRhiDevice)
	{
		m_wpRHIDevice = _spRhiDevice;
	}

	virtual void SetViewport(const RectangleI& _rect, float _minDepth = 0.0f,
							 float _maxDepth = 1.0f) = 0;

protected:
	std::weak_ptr<RHI_Device> m_wpRHIDevice;
};
} // namespace Miyadaiku