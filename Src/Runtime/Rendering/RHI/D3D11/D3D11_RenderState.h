#pragma once
#include "../RHI_RenderState.h"

namespace Miyadaiku
{
struct D3D11_SamplerState : public RHI_SamplerState
{
public:
	D3D11_SamplerState(const std::shared_ptr<RHI_Device>& _spRHIDevice,
					   const SS_FilterMode				  _filterMode,
					   const SS_AddressMode				  _addressMode);
	~D3D11_SamplerState();

private:
};

struct D3D11_RasterizerState : public RHI_RasterizerState
{

	D3D11_RasterizerState(const std::shared_ptr<RHI_Device>& _spRHIDevice,
						  const RS_CullMode					 _cullMode,
						  const RS_FillMode					 _fillMode);
	~D3D11_RasterizerState();
};

} // namespace Miyadaiku