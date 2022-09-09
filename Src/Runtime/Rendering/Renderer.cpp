#include "Renderer.h"
#include "../Core/Engine.h"
#include "../Core/Platform/Platform.h"
#include "../Core/Platform/Windows/Window_Windows.h"
#include "RHI/D3D11/D3D11_Device.h"
#include "RHI/D3D11/D3D11_SwapChain.h"
#include "RHI/D3D11/D3D11_Shader.h"

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

	//pDevivceContext->ClearDepthStencilView(
	//	m_spDepthStencilBuffer->DSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
	//	1, 0);

	m_spRHISwapChain->Present();
}
} // namespace Miyadaiku