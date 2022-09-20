#pragma once
#include "../RenderingResource.h"
#include <memory>

namespace Miyadaiku
{

//-----------------------
// SamplerState enums
//-----------------------

// サンプラーステート：FilterMode
// 補完の種類
enum class SS_FilterMode
{
	Point = 0x0000001,	// 補完無し
	Linear = 0x0000002, // 線形補完
	Aniso = 0x0000004,	// 違方向性
};

// サンプラーステート：AddressMode
// UV座標がはみ出した際、どのように扱うか？
enum class SS_AddressMode
{
	Wrap = 0x00010000,	 // ループする
	Clamp = 0x00020000,	 // 範囲内におさめる
	Mirror = 0x00040000, // 折り返す
};
//-----------------------
// BlendState enums
//-----------------------
enum class BlendMode
{
	Add,
	Alpha,
	None
};
//-----------------------
// RasterizerState enums
//-----------------------
enum class RS_CullMode
{
	None = 0x0000001,
	Back = 0x0000002,
	Front = 0x0000004,
};
enum class RS_FillMode
{
	Wire = 0x0001000,
	Solid = 0x0002000,
};

enum class RHI_PrimitiveTopology
{
	TriangleList,
	LineList,
};

//========================================
class RHI_Device;
//========================================

struct RHI_SamplerState
{
	RHI_SamplerState(const std::shared_ptr<RHI_Device>& _spRHIDevice,
					 const SS_FilterMode				_filterMode,
					 const SS_AddressMode				_addressMode)
	{
		m_wpDevice = _spRHIDevice;
		m_filterMode = _filterMode;
		m_addressMode = _addressMode;
	}

	RHIResourceHandle_SamplerState& GetResourceHandle()
	{
		return m_resourceHandle;
	}

	SS_FilterMode  m_filterMode = SS_FilterMode::Aniso;
	SS_AddressMode m_addressMode = SS_AddressMode::Wrap;

protected:
	RHIResourceHandle_SamplerState m_resourceHandle;

	std::weak_ptr<RHI_Device> m_wpDevice;
};

struct RHI_BlendState
{
	RHI_BlendState(const std::shared_ptr<RHI_Device>& _spRHIDevice,
				   const BlendMode					  _blendMode)
	{
		m_wpDevice = _spRHIDevice;
		m_blendMode = _blendMode;
	}

	RHIResourceHandle_BlendState& GetResourceHandle()
	{
		return m_resourceHandle;
	}

	BlendMode m_blendMode = BlendMode::Alpha;

protected:
	RHIResourceHandle_BlendState m_resourceHandle;

	std::weak_ptr<RHI_Device> m_wpDevice;
};

struct RHI_RasterizerState
{
	RHI_RasterizerState(const std::shared_ptr<RHI_Device>& _spRHIDevice,
						const RS_CullMode				   _cullMode,
						const RS_FillMode				   _fillMode)
	{
		m_wpDevice = _spRHIDevice;
		m_cullMode = _cullMode;
		m_fillMode = _fillMode;
	}

	RHIResourceHandle_RasterizerState& GetResourceHandle()
	{
		return m_resourceHandle;
	}

	RS_CullMode m_cullMode = RS_CullMode::Back;
	RS_FillMode m_fillMode = RS_FillMode::Solid;

protected:
	RHIResourceHandle_RasterizerState m_resourceHandle;

	std::weak_ptr<RHI_Device> m_wpDevice;
};

struct RHI_DepthStencilState
{
	RHI_DepthStencilState(const std::shared_ptr<RHI_Device>& _spRHIDevice,
						  bool _zUse,bool _zWrite)
	{
		m_wpDevice = _spRHIDevice;
		m_zUse = _zUse;
		m_zWrite = _zWrite;
	}

	RHIResourceHandle_DepthStencilState& GetResourceHandle()
	{
		return m_resourceHandle;
	}
	bool								m_zUse = true;
	bool								m_zWrite = true;

protected:
	RHIResourceHandle_DepthStencilState m_resourceHandle;
	std::weak_ptr<RHI_Device>			m_wpDevice;
};

} // namespace Miyadaiku