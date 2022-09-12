#include "D3D11_CommandList.h"
#include "D3D11Common.h"
#include "D3D11_Device.h"

namespace Miyadaiku
{
void D3D11_CommandList::SetViewport(const RectangleI& _rect, float _minDepth,
									float _maxDepth)
{
	std::shared_ptr<D3D11_Device> spDevice =
		std::static_pointer_cast<D3D11_Device>(m_wpRHIDevice.lock());

	D3D11_VIEWPORT vp = {};
	vp.TopLeftX = (float)_rect.left;
	vp.TopLeftY = (float)_rect.top;
	vp.Width = (float)_rect.width;
	vp.Height = (float)_rect.height;
	vp.MinDepth = _minDepth;
	vp.MaxDepth = _maxDepth;

	spDevice->GetDeviceContext()->RSSetViewports(1, &vp);
}
} // namespace Miyadaiku