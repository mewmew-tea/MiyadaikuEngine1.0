#pragma once
#include <memory>

namespace Miyadaiku
{
class RHI_Shader;
class RHI_InputLayout
{
public:


protected:
	std::weak_ptr<RHI_Shader> m_wpRHIVertexShader;	// 対象の頂点シェーダ
};
} // namespace Miyadaiku