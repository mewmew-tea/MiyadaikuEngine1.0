#pragma once
#include "../RHI_Shader.h"
#include "D3D11Common.h"

namespace Miyadaiku
{
class D3D11_Shader : public RHI_Shader
{
public:
	D3D11_Shader(const std::shared_ptr<RHI_Device>& _spRhiDevice)
		: RHI_Shader(_spRhiDevice)
	{
	}

	bool Load(std::string_view _filePath, ShaderType _type,
			  std::string_view _entryFuncName) override;

	/**
	 * シェーダのID3DBlob(バイナリコード)を返します。
	 * Load関数内で作成されますが、Load関数終了時にはnullptrになります。
	 * おもにVertexShaderでのInputLayoutやConstantBuffer向けです。
	 *
	 * @return シェーダのID3DBlob(バイナリコード)
	 */
	ID3DBlob* GetShaderID3DBlob()
	{
		return m_pShaderBlob;
	}

private:
	ID3DBlob*		   m_pShaderBlob = nullptr;
	ID3D11InputLayout* m_pInputLayout = nullptr;
};
} // namespace Miyadaiku