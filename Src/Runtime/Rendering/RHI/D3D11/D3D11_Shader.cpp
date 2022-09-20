#include "D3D11_Shader.h"
#include "../../../External/strconv/strconv.h"
#include "D3D11_Device.h"

#include <iostream>
#include <cassert>
#include <sstream>
#include <string>
#include <filesystem>

namespace Miyadaiku
{
D3D11_Shader::~D3D11_Shader()
{
	D3D11_SafeCastRelease<ID3D11PixelShader>(m_dataHandle.m_pData);
}
bool D3D11_Shader::Load(std::string_view _filePath, ShaderType _type,
						std::string_view _entryFuncName)
{

	char cdir[255];
	GetCurrentDirectory(255, cdir);
	std::cout << "Current Directory : " << cdir << std::endl;

	if (!std::filesystem::exists(_filePath))
	{
		assert(0 && "シェーダファイルが存在しない");
	}

	//-------------------------------
	// コンパイル
	//-------------------------------
	std::string shaderModelName;
	switch (_type)
	{
	case ShaderType::Vertex:
		//shaderModelName = "vs_4_0";
		shaderModelName = "vs_4_0_level_9_3";
		break;

	case ShaderType::Pixel:
		shaderModelName = "ps_4_0_level_9_3";
		break;

	default:
		assert(0 && "未対応のShaderType");
		return false;
		break;
	}

	ID3DBlob*	 pBlobError = nullptr;
	ID3DBlob*	 pShaderBlob = nullptr;
	HRESULT		 hr;
	std::wstring wPath = utf8_to_wide(std::string(_filePath.data()));
	DWORD		 dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// デバッガ用に、シェーダー最適化無効化
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	hr = D3DCompileFromFile(wPath.c_str(), nullptr,
							D3D_COMPILE_STANDARD_FILE_INCLUDE,
							_entryFuncName.data(), shaderModelName.c_str(),
							dwShaderFlags, 0, &pShaderBlob, &pBlobError);

	if (pBlobError)
	{
		OutputDebugStringA(
			reinterpret_cast<const char*>(pBlobError->GetBufferPointer()));

		pBlobError->Release();
		pBlobError = nullptr;
	}

	//----------------------
	// シェーダを作成
	//----------------------
	std::shared_ptr<D3D11_Device> spDevice =
		std::static_pointer_cast<D3D11_Device>(m_wpRHIDevice.lock());

	void* pShaderView = nullptr;
	if (pShaderBlob)
	{
		switch (_type)
		{
		case ShaderType::Vertex:
			spDevice->GetDevice()->CreateVertexShader(
				pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(),
				nullptr, reinterpret_cast<ID3D11VertexShader**>(&pShaderView));
			// input layout作成
			// TODO: InputLayoutクラスを使う。
			// m_wpRHIInputLayout->Create(m_vertex_type, shader_blob);
			{
				//// 頂点レイアウト
				// SemanticName, SemanticIndex, Format, InputSlot,
				// AlignedByteOffset, InputSlotClass, InstanceDataStepRate
				const D3D11_INPUT_ELEMENT_DESC layout[] =
				{
					{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
					 D3D11_INPUT_PER_VERTEX_DATA, 0},
					{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12,
					 D3D11_INPUT_PER_VERTEX_DATA, 0},
					{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 20,
					 D3D11_INPUT_PER_VERTEX_DATA, 0},
				};

				if (FAILED(spDevice->GetDevice()->CreateInputLayout(
					layout, ARRAYSIZE(layout), pShaderBlob->GetBufferPointer(),
					pShaderBlob->GetBufferSize(), &m_pInputLayout)))
				{
						assert(0 && "頂点レイアウト作成失敗");
						return false;
				}
			}
			break;

		case ShaderType::Pixel:
			// ピクセルシェーダー作成
			spDevice->GetDevice()->CreatePixelShader(
				pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(),
				nullptr, reinterpret_cast<ID3D11PixelShader**>(&pShaderView));

			break;

		default:
			assert(0 && "未対応のShaderType");
			return false;
			break;
		}

		pShaderBlob->Release();
		pShaderBlob = nullptr;
	}

	m_dataHandle.m_pData = pShaderView;
	
	return true;
}
} // namespace Miyadaiku

