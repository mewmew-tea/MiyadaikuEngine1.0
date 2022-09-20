#pragma once
#include <cassert>

namespace Miyadaiku
{

// enum class RHIDataSemantic
//{
//     Position = 0,
//     Normal,
//     Tangent,
//     BiTangent,
//     Color,
//     TexCoord,
//     BlendIndex,
//     BlendWeight,
//
//     None,
// };

enum class RHIResourceType
{
	None,

	Texture, // TextureResource
	SRV,	 // ShaderResourceView
	UAV,	 //
	RT,		 // RenderTargetView
	DS,		 // DepthStencil

	SwapChain, // 例：DXGISwapChain

	Shader,

	BlendState,
	RasterizerState,
	SamplerState,
	DepthStencilState,

	Max_,
};

// グラフィックスAPIのオブジェクトのポインタを扱うためのハンドル
template <RHIResourceType T> struct RHIResourceHandle
{
public:
	RHIResourceHandle() = default;

	~RHIResourceHandle()
	{
		assert(m_pData == nullptr);
	}

	bool IsValid() const
	{
		return m_pData != nullptr;
	}

	void Reset()
	{
		m_pData = nullptr;
	}

	void** GetAddress()
	{
		return &m_pData;
	}

	RHIResourceType Type()
	{
		return T;
	}

	inline bool operator=(RHIResourceHandle const& _rhs) = delete;
	inline bool operator==(RHIResourceHandle const& _rhs) const
	{
		return m_pData == _rhs.m_pData;
	}
	inline bool operator!=(RHIResourceHandle const& _rhs) const
	{
		return m_pData != _rhs.m_pData;
	}

	void* m_pData = nullptr;
};


typedef RHIResourceHandle<RHIResourceType::Texture>			RHIResourceHandle_Texture;
typedef RHIResourceHandle<RHIResourceType::SRV>				RHIResourceHandle_SRV;
typedef RHIResourceHandle<RHIResourceType::RT>				RHIResourceHandle_RT;
typedef RHIResourceHandle<RHIResourceType::DS>				RHIResourceHandle_DS;
typedef RHIResourceHandle<RHIResourceType::SwapChain>		RHIResourceHandle_SwapChain;
typedef RHIResourceHandle<RHIResourceType::Shader>			RHIResourceHandle_Shader;

typedef RHIResourceHandle<RHIResourceType::RasterizerState>	RHIResourceHandle_RasterizerState;
typedef RHIResourceHandle<RHIResourceType::BlendState>		RHIResourceHandle_BlendState;
typedef RHIResourceHandle<RHIResourceType::SamplerState>	RHIResourceHandle_SamplerState;
typedef RHIResourceHandle<RHIResourceType::DepthStencilState>	RHIResourceHandle_DepthStencilState;

} // namespace Miyadaiku