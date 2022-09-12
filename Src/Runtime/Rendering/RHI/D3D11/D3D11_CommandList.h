#pragma once
#include "../RHI_CommandList.h"

namespace Miyadaiku
{
class D3D11_CommandList : public RHI_CommandList
{
public:
	D3D11_CommandList(const std::shared_ptr<RHI_Device>& _spRhiDevice)
		: RHI_CommandList(_spRhiDevice)
	{
	}

	void SetViewport(const RectangleI& _rect, float _minDepth = 0.0f,
					 float _maxDepth = 1.0f) override;
};
} // namespace Miyadaiku
