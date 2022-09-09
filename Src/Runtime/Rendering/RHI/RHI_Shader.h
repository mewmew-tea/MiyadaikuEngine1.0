#pragma once
#include <memory>
#include <string_view>

namespace Miyadaiku
{
class RHI_Device;
class RHI_InputLayout;

class RHI_Shader
{
public:
	RHI_Shader(const std::shared_ptr<RHI_Device>& _spRhiDevice)
	{
		m_wpRHIDevice = _spRhiDevice;
	}

	enum class ShaderType
	{
		Vertex,
		Pixel,
		// Compute,
	};

	virtual bool Load(std::string_view _filePath, ShaderType _type,
					  std::string_view _entryFuncName) = 0;

protected:


	std::shared_ptr<RHI_InputLayout> m_wpRHIInputLayout;	// 頂点シェーダの場合のための、InputLayout
	std::weak_ptr<RHI_Device>		 m_wpRHIDevice;			

private:
};
} // namespace Miyadaiku