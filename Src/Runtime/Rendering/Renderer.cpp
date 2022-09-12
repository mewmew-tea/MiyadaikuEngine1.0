#include "Renderer.h"
#include "../Core/Engine.h"
#include "../Core/Platform/Platform.h"
#include "../Core/Platform/Windows/Window_Windows.h"

#include "../Core/Math/Vector2.h"
#include "../Core/Math/Vector3.h"

#include "RHI/D3D11/D3D11_Device.h"
#include "RHI/D3D11/D3D11_SwapChain.h"
#include "RHI/D3D11/D3D11_CommandList.h"
#include "RHI/D3D11/D3D11_Shader.h"
#include "RHI/D3D11/D3D11_Texture.h"

namespace Miyadaiku
{
Renderer::~Renderer()
{
}
void Renderer::OnAwake()
{
	m_spRHIDevice = std::make_shared<D3D11_Device>();
	// m_device = new D3D11_Device();
	if (m_spRHIDevice)
	{
		m_spRHIDevice->Init();
	}

	std::shared_ptr<Window_Windows> window =
		static_pointer_cast<Window_Windows>(GetEngine()
												->GetSubsystemLocator()
												.Get<Platform>()
												->GetMainWindow());

	m_spRHIComandList = std::make_shared<D3D11_CommandList>(m_spRHIDevice);
	if (m_spRHIComandList)
	{
		RectangleI rect = window->GetRect();
		rect.left = 0;
		rect.top = 0;
		m_spRHIComandList->SetViewport(rect);
	}

	// m_swapChain = std::make_shared<D3D11_SwapChain>(
	//	window->GetWindowHandle(), m_device, //
	//	window->GetRect().width, window->GetRect().height);
	m_spRHISwapChain = std::make_shared<D3D11_SwapChain>();
	if (m_spRHISwapChain)
	{
		m_spRHISwapChain->Init(window->GetWindowHandle(), m_spRHIDevice,
							   window->GetRect().width,
							   window->GetRect().height);
	}

	m_spRHIVertexShader = std::make_shared<D3D11_Shader>(m_spRHIDevice);
	if (m_spRHIVertexShader)
	{
		m_spRHIVertexShader->Load("Assets/Shaders/Unlit_VS.hlsl",
								  RHI_Shader::ShaderType::Vertex, "main");
	}
	m_spRHIPixelShader = std::make_shared<D3D11_Shader>(m_spRHIDevice);
	if (m_spRHIPixelShader)
	{
		m_spRHIPixelShader->Load("Assets/Shaders/Unlit_PS.hlsl",
								  RHI_Shader::ShaderType::Pixel, "main");
	}


	//00000000000000000000000000000000000
	ID3D11Device* pDevivce =
		std::static_pointer_cast<D3D11_Device>(m_spRHIDevice)->GetDevice();
	ID3D11DeviceContext* pDevivceContext =
		std::static_pointer_cast<D3D11_Device>(m_spRHIDevice)
			->GetDeviceContext();

	D3D11_SAMPLER_DESC smpDesc = {};
	smpDesc.MaxLOD = D3D11_FLOAT32_MAX;
		smpDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		// smpDesc.MaxAnisotropy = 4;
		smpDesc.AddressU = smpDesc.AddressV = smpDesc.AddressW =
			D3D11_TEXTURE_ADDRESS_WRAP;
	ID3D11SamplerState* smpState;
		pDevivce->CreateSamplerState(
		&smpDesc, &smpState);
	pDevivceContext->PSSetSamplers(0, 1, &smpState);
		pDevivceContext->VSSetSamplers(0, 1, &smpState);
}
void Renderer::OnShutdown()
{
	m_spRHIDevice->Release();
	m_spRHISwapChain->Release();
}
void Renderer::Present()
{
	// TODO: SwapChain->Present以外を、RHI_CommandListに移行
	// TODO: DepthStencil関連の処理

	ID3D11DeviceContext* pDevivceContext =
		std::static_pointer_cast<D3D11_Device>(m_spRHIDevice)
			->GetDeviceContext();

	ID3D11Device* pDevivce =
		std::static_pointer_cast<D3D11_Device>(m_spRHIDevice)
			->GetDevice();

	// std::shared_ptr<D3D11_SwapChain> spRHISwapChain =
	//	std::static_pointer_cast<D3D11_SwapChain>(m_spRHISwapChain);

	ID3D11RenderTargetView* rtv = reinterpret_cast<ID3D11RenderTargetView*>(
		(m_spRHISwapChain->GetBackBuffer()->GetRTHandle().m_pData));

	ID3D11RenderTargetView* rtvs[] = {rtv};

	pDevivceContext->OMSetRenderTargets(
		1, rtvs, nullptr /*m_spDepthStencilBuffer->DSV()*/);

	// 試験的に、バックバッファクリアするたびに色を変化させる。
	float clearColor[4];
	for (int i = 0; i < 4; ++i)
	{
		clearColor[i] = 0.0f;
	}
	static float r = 0.0f;
	static float n = 0.003f;
	r += n;
	if (r > 1.0f || r < 0.0f)
	{
		n = -n;
	}
	clearColor[0] = r;

	pDevivceContext->ClearRenderTargetView(rtv, clearColor);

	std::shared_ptr<D3D11_Texture> spTex = std::make_shared<D3D11_Texture>(m_spRHIDevice);
	spTex->Create("Assets/Textures/4.png");
	pDevivceContext->PSSetShaderResources(0, 1, (ID3D11ShaderResourceView**)&(spTex->GetSRVHandle().m_pData));

	std::shared_ptr<D3D11_Shader> spPixelShader =
		std::static_pointer_cast<D3D11_Shader>(m_spRHIPixelShader);
	std::shared_ptr<D3D11_Shader> spVertexShader =
		std::static_pointer_cast<D3D11_Shader>(m_spRHIVertexShader);

	pDevivceContext->VSSetShader((ID3D11VertexShader*)spVertexShader->GetDataHandle().m_pData, 0,0);
	pDevivceContext->PSSetShader((ID3D11PixelShader*)spPixelShader->GetDataHandle().m_pData, 0,0);
	pDevivceContext->IASetInputLayout(spVertexShader->GetInputLayout());


	struct TestVertex
	{

		TestVertex(const Vector3& _pos, const Vector2& _uv, UINT _color)
		{
			Pos = _pos;
			UV = _uv;
			Color = _color;
		}
		Vector3 Pos = {};
		Vector2 UV = {};
		UINT			  Color = 0xFFFFFFFF;
	};

	UINT					oneSize = sizeof(TestVertex);
	ID3D11Buffer* pBuffer = nullptr;
	std::vector<TestVertex> vertex;
	vertex.push_back(TestVertex({0, 0, 0}, {0, 1}, 0xFFFFFFFF));
	vertex.push_back(TestVertex({0, 1, 0}, {0, 0}, 0xFFFFFFFF));
	vertex.push_back(TestVertex({1, 0, 0}, {1, 1}, 0xFFFFFFFF));
	vertex.push_back(TestVertex({1, 1, 0}, {1, 0}, 0xFFFFFFFF));

	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = sizeof(TestVertex) * (UINT)vertex.size();
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = &vertex[0];

	HRESULT hr =
		pDevivce->CreateBuffer(&desc, &vertex[0] ? &data : nullptr, &pBuffer);

	// 頂点リストの並び順
	pDevivceContext->IASetPrimitiveTopology(
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// GPUへ転送・描画
	UINT offset = 0;
	pDevivceContext->IASetVertexBuffers(0, 1, &pBuffer, &oneSize, &offset);
	pDevivceContext->Draw((UINT)vertex.size(), 0);

	pBuffer->Release();

	//pDevivceContext->ClearDepthStencilView(
	//	m_spDepthStencilBuffer->DSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
	//	1, 0);



	m_spRHISwapChain->Present();
}
} // namespace Miyadaiku