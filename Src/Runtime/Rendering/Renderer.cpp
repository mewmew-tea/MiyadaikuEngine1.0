﻿#include "Renderer.h"
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
#include "RHI/D3D11/D3D11_RenderState.h"

#include <memory>
#include <iostream>

// TODO: これは取り除く
#include <mono/metadata/object.h>

#include "../Scripting/Scripting.h"
#include "../Scripting/Component.h"
#include "../Scripting/ScriptPropertyInfo.h"

#include "imgui.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_PLACEMENT_NEW

#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "imgui_internal.h"
//#include "imgui_stdlib.h"

//// ImGuizmo
//#include "ImGuizmo.h"
//#include "ImSequencer.h"
//#include "ImZoomSlider.h"
//#include "ImCurveEdit.h"
//#include "GraphEditor.h"


namespace Miyadaiku
{
Renderer::~Renderer()
{
}
void Renderer::OnAwake()
{
	std::shared_ptr<Window_Windows> window =
		static_pointer_cast<Window_Windows>(GetEngine()
												->GetSubsystemLocator()
												.Get<Platform>()
												->GetMainWindow());
	// Create device
	m_spRHIDevice = std::make_shared<D3D11_Device>();
	if (m_spRHIDevice)
	{
		m_spRHIDevice->Init();
	}
	
	// Create command list
	m_spRHIComandList = std::make_shared<D3D11_CommandList>(m_spRHIDevice);
	if (m_spRHIComandList)
	{
		RectangleI rect = window->GetRect();
		rect.left = 0;
		rect.top = 0;
		m_spRHIComandList->SetViewport(rect);
	}

	// Create swapchain
	m_spRHISwapChain = std::make_shared<D3D11_SwapChain>();
	if (m_spRHISwapChain)
	{
		m_spRHISwapChain->Init(window->GetWindowHandle(), m_spRHIDevice,
							   window->GetRect().width,
							   window->GetRect().height);
	}

	// Create shaders
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

	// Create sampler states
	m_spRHISamplerState = std::make_shared<D3D11_SamplerState>(m_spRHIDevice, SS_FilterMode::Aniso, SS_AddressMode::Wrap);
	m_spRHIComandList->SetSamplerState(0, m_spRHISamplerState);
	// Create rasterize state
	m_spRHIRasterizerState = std::make_shared<D3D11_RasterizerState>(m_spRHIDevice, RS_CullMode::Back, RS_FillMode::Solid);



	//===================================================================
	// imgui初期設定
	//===================================================================
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	auto& subsystemLocator = GetEngine()->GetSubsystemLocator();
	auto mainWindow = std::static_pointer_cast<Window_Windows>(
		 subsystemLocator.Get<Platform>()->GetMainWindow());
	auto device = std::static_pointer_cast<D3D11_Device>(
		m_spRHIDevice);

	ImGuiIO& io = ImGui::GetIO();
	// Setup Dear ImGui style
	ImGui::StyleColorsClassic();

	{
		auto& io = ImGui::GetIO();
		 io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           //
		// Docking有効化 io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		// // マルチビューポート有効化
	}
	// Setup Platform/Renderer bindings
	void* editorHwnd = GetEngine()->m_editorHWnd;

	ImGui_ImplWin32_Init(editorHwnd ? editorHwnd
									  : mainWindow->GetWindowHandle());
	ImGui_ImplDX11_Init(device->GetDevice(), device->GetDeviceContext());

	{
		// 日本語対応
#include "ja_glyph_ranges.h"
		ImFontConfig config;
		//config.MergeMode = true;
		io.Fonts->AddFontDefault();
		//std::string fontPath = "c:\\Windows\\Fonts\\msgothic.ttc";
		//std::string fontPath = "c:\\Windows\\Fonts\\meiryo.ttc";
		std::string fontPath = ".\\Assets\\EngineResources\\fonts\\GenShinGothic-Monospace-Bold.ttf";
		// std::string fontPath = ".\\Resources\\Fonts\\ipaexg.ttf";
		// std::string fontPath = ".\\Resources\\Fonts\\ipag.ttf";
		auto font = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 21.0f, &config,
									 glyphRangesJapanese);
		io.Fonts->Build();
		io.FontDefault = font;
	}
}
void Renderer::OnShutdown()
{
	// imgui解放
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	m_spRHIPixelShader.reset();
	m_spRHIVertexShader.reset();
	m_spRHISamplerState.reset();


	m_spRHIDevice->Release();
	m_spRHISwapChain->Release();
}
void Renderer::Tick()
{
}
void Renderer::Present()
{
	// TODO: SwapChain->Present以外を、RHI_CommandListに移行
	// TODO: DepthStencil関連の処理

	std::shared_ptr<Window_Windows> window =
		static_pointer_cast<Window_Windows>(GetEngine()
												->GetSubsystemLocator()
												.Get<Platform>()
												->GetMainWindow());


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
	static float g = 0.0f;
	static float n = 0.003f;
	g += n;
	if (g > 1.0f || g < 0.0f)
	{
		n = -n;
	}
	clearColor[1] = g;
	{
	//RHI_PipelineState pso = {};
	//pso.m_spVertexShader = m_spRHIVertexShader;
	//pso.m_spPixelShader = m_spRHIPixelShader;
	//pso.m_spRasterizerState = m_spRHIRasterizerState;
	////pso.m_spBlendState = ;
	////pso.m_spDepthStencilState;
	//pso.m_primitiveTopology = RHI_PrimitiveTopology::TriangleList;
	//
	//RectangleI rect = window->GetRect();
	//rect.left = 0;
	//rect.top = 0;
	//m_spRHIComandList->SetViewport(rect);
	//pso.m_viewportRect = rect;

	////pso.m_spDepthStencilBuffer;
	////pso.m_spRenderTargets;
	////pso.m_spRenderTargetClearColors;

	//m_spRHIComandList->SetPipelineState(pso);
	}


	pDevivceContext->ClearRenderTargetView(rtv, clearColor);

	static std::shared_ptr<D3D11_Texture> spTex;
	if (!spTex)
	{
		spTex= std::make_shared<D3D11_Texture>(m_spRHIDevice);
		spTex->Create("Assets/Textures/4.png");
	}
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
	//vertex.push_back(TestVertex({0, 0, 0}, {0, 1}, 0xFFFFFFFF));
	//vertex.push_back(TestVertex({0, 1, 0}, {0, 0}, 0xFFFFFFFF));
	//vertex.push_back(TestVertex({1, 0, 0}, {1, 1}, 0xFFFFFFFF));
	//vertex.push_back(TestVertex({1, 1, 0}, {1, 0}, 0xFFFFFFFF));

	//Vector3 pos = {ScriptingAPI_GetPositionX(), ScriptingAPI_GetPositionY(), 0};
	//pos /= 4.0f;
	//Vector3 pos;
	auto spScripting = GetEngine()->GetSubsystemLocator().Get<Scripting>();
	
	for (auto go : spScripting->GetGameObjects())
	{
		Vector3 pos;
		auto	transform = go->GetTransform();
		for (auto spProp : transform->GetClassInstance()->m_pClassType->spPropertyInfos)
		{
			if (spProp->name == "Position")
			{
				Vector3 ret;
				spProp->GetValue(
					transform->GetClassInstance()->m_pInstance, &ret);
				//if (ret != nullptr)
				{
					pos = ret;
					//std::cout << pos.x << ", " << pos.y << std::endl;
				}
				break;
			}
		}
		vertex.clear();
		vertex.push_back(TestVertex({pos.x + 0, pos.y + 0, 0}, {0, 1}, 0xFFFFFFFF));
		vertex.push_back(TestVertex({pos.x + 0, pos.y + 1, 0}, {0, 0}, 0xFFFFFFFF));
		vertex.push_back(TestVertex({pos.x + 1, pos.y + 0, 0}, {1, 1}, 0xFFFFFFFF));
		vertex.push_back(TestVertex({pos.x + 1, pos.y + 1, 0}, {1, 0}, 0xFFFFFFFF));


		D3D11_BUFFER_DESC desc = {};
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = sizeof(TestVertex) * (UINT)vertex.size();
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA data = {};
		data.pSysMem = &vertex[0];

		HRESULT hr =
			pDevivce->CreateBuffer(&desc, &vertex[0] ? &data : nullptr, &pBuffer);

		//// 頂点リストの並び順
		pDevivceContext->IASetPrimitiveTopology(
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// GPUへ転送・描画
		UINT offset = 0;
		pDevivceContext->IASetVertexBuffers(0, 1, &pBuffer, &oneSize, &offset);
		//pDevivceContext->Draw((UINT)vertex.size(), 0);
		//pDevivceContext->DrawInstanced((UINT)vertex.size(), 1, 0, 0);
		m_spRHIComandList->Draw((uint16_t)vertex.size(), 0);
		pBuffer->Release();
	}

	//pDevivceContext->ClearDepthStencilView(
	//	m_spDepthStencilBuffer->DSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
	//	1, 0);
	
	
    //--------------------------------
	// ImGui
	//--------------------------------
	// begin
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// imgui update(script)
	auto& subsystemLocator = GetEngine()->GetSubsystemLocator();
	auto  scripting = subsystemLocator.Get<Scripting>();
	scripting->ImGuiUpdate();

	// ImGui render
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	auto& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	m_spRHISwapChain->Present();
}
} // namespace Miyadaiku