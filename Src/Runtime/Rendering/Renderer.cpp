#include "Renderer.h"

#include "../Core/Engine.h"

#include "../Effect/Effect.h"

#include "../Core/Platform/Platform.h"
#include "../Core/Platform/Windows/Window_Windows.h"

#include "../Core/Math/Vector2.h"
#include "../Core/Math/Vector3.h"

#include "Model.h"

#include "RHI/D3D11/D3D11_Device.h"
#include "RHI/D3D11/D3D11_SwapChain.h"
#include "RHI/D3D11/D3D11_CommandList.h"
#include "RHI/D3D11/D3D11_Shader.h"
#include "RHI/D3D11/D3D11_Texture.h"
#include "RHI/D3D11/D3D11_RenderState.h"
#include "RHI/D3D11/D3D11_ConstantBuffer.h"
#include "RHI/D3D11/D3D11_VertexBuffer.h"
#include "RHI/D3D11/D3D11_IndexBuffer.h"
#include "CommonConstantBuffer.h"

#include <memory>
#include <iostream>

// TODO: これは取り除く
#include <mono/metadata/object.h>

#include "../Scripting/Scripting.h"
#include "../Scripting/Component.h"
#include "../Scripting/ScriptPropertyInfo.h"

#pragma warning(push)
#pragma warning(disable : 6011)
#pragma warning(disable : 6387)
#pragma warning(disable : 28182)
#pragma warning(disable : 33010) 
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
#pragma warning(pop)

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
	m_spRHICommandList = std::make_shared<D3D11_CommandList>(m_spRHIDevice);
	if (m_spRHICommandList)
	{
		RectangleI rect = window->GetRect();
		rect.left = 0;
		rect.top = 0;
		m_spRHICommandList->SetViewport(rect);
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
	m_spRHICommandList->SetSamplerState(0, m_spRHISamplerState);
	// Create rasterize state
	m_spRHIRasterizerState = std::make_shared<D3D11_RasterizerState>(m_spRHIDevice, RS_CullMode::Back, RS_FillMode::Solid);

	// Crete constant buffers
	// camera
	m_spCbCamera = std::make_shared<D3D11_ConstantBuffer>(m_spRHIDevice);
	if (!m_spCbCamera->Create<Cb_Camera>())
	{
		assert(0 && "Failed Create constant buffer!");
	}
	m_cbCameraData.mView =
		Matrix::CreateRotationZ(30.0f * (3.1415926535f / 180.0f))
		* Matrix::CreateTranslation(0.0f, 0.7f, -1.0f);
	m_cbCameraData.mView.Invert();
	m_cbCameraData.mProj = Matrix::CreatePerspectiveFovLH(
		60.0f * (3.1415926535f / 180.0f), 16.0f / 9.0f, 0.01f, 2000.0f);
	// uber
	m_spCbUber = std::make_shared<D3D11_ConstantBuffer>(m_spRHIDevice);
	if (!m_spCbUber->Create<Cb_Uber>())
	{
		assert(0 && "Failed Create constant buffer!");
	}
	// material
	m_spCbMaterial = std::make_shared<D3D11_ConstantBuffer>(m_spRHIDevice);
	if (!m_spCbMaterial->Create<Cb_Material>())
	{
		assert(0 && "Failed Create constant buffer!");
	}
	// light
	m_spCbLight = std::make_shared<D3D11_ConstantBuffer>(m_spRHIDevice);
	if (!m_spCbLight->Create<Cb_Light>())
	{
		assert(0 && "Failed Create constant buffer!");
	}


	// Create default texture
	{
		// white: 1, 1, 1, 1
		// 0xAABBGGRR
		uint32_t col = 0;
		col |= (uint8_t(UINT8_MAX) << 8 * 0);
		col |= (uint8_t(UINT8_MAX) << 8 * 1);
		col |= (uint8_t(UINT8_MAX) << 8 * 2);
		col |= (uint8_t(UINT8_MAX) << 8 * 3);;

		m_spWhiteTexure = std::make_shared<D3D11_Texture>(m_spRHIDevice);
		if (!m_spWhiteTexure->Create(1, 1, RHI_FORMAT::R8G8B8A8_UNORM, 1, &col,
									 4))
		{
			assert(0 && "Failed to default white texture.");
		}
	}
	{
		// normal: 0.5, 0.5, 1, 1
		// 0xAABBGGRR
		uint32_t col = 0;
		col |= (uint8_t(UINT8_MAX / 2) << 8 * 0);
		col |= (uint8_t(UINT8_MAX / 2) << 8 * 1);
		col |= (uint8_t(UINT8_MAX) << 8 * 2);
		col |= (uint8_t(UINT8_MAX) << 8 * 3);

		m_spNormalTexure = std::make_shared<D3D11_Texture>(m_spRHIDevice);
		if (!m_spNormalTexure->Create(1, 1, RHI_FORMAT::R8G8B8A8_UNORM, 1, &col,
									 4))
		{
			assert(0 && "Failed to default normal texture.");
		}
	}


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

	// Setup Dear ImGui style
	ImGui::StyleColorsClassic();

	{
		auto& io = ImGui::GetIO();
		 io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           //
		// Docking有効化 io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		// // マルチビューポート有効化
	}
	// Setup Platform/Renderer bindings

	ImGui_ImplWin32_Init(mainWindow->GetWindowHandle());
	ImGui_ImplDX11_Init(device->GetDevice(), device->GetDeviceContext());

	{
		// 日本語対応
#include "ja_glyph_ranges.h"
		ImFontConfig config;
		auto&		 io = ImGui::GetIO();
		// config.MergeMode = true;
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


	ID3D11DeviceContext* pDeviceContext = nullptr;
	pDeviceContext = std::static_pointer_cast<D3D11_Device>(m_spRHIDevice)
			->GetDeviceContext();
	if (!pDeviceContext)
	{
		return;
	}

	//ID3D11Device* pDevice = nullptr;
	//pDevice = std::static_pointer_cast<D3D11_Device>(m_spRHIDevice)
	//		->GetDevice();
	//if (!pDevice)
	//{
	//	return;
	//}

	// std::shared_ptr<D3D11_SwapChain> spRHISwapChain =
	//	std::static_pointer_cast<D3D11_SwapChain>(m_spRHISwapChain);

	ID3D11RenderTargetView* rtv = reinterpret_cast<ID3D11RenderTargetView*>(
		(m_spRHISwapChain->GetBackBuffer()->GetRTHandle().m_pData));

	ID3D11RenderTargetView* rtvs[] = {rtv};

	pDeviceContext->OMSetRenderTargets(
		1, rtvs,
		reinterpret_cast<ID3D11DepthStencilView*> (m_spRHISwapChain
			->GetDepthStencilBuffer()
			->GetDSHandle().m_pData)
		);

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


	pDeviceContext->ClearRenderTargetView(rtv, clearColor);
	pDeviceContext->ClearDepthStencilView(
		reinterpret_cast<ID3D11DepthStencilView*>(
			m_spRHISwapChain->GetDepthStencilBuffer()->GetDSHandle().m_pData),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1, 0);

	static std::shared_ptr<D3D11_Texture> spTex;
	if (!spTex)
	{
		spTex= std::make_shared<D3D11_Texture>(m_spRHIDevice);
		spTex->Create("Assets/Textures/4.png");
	}
	pDeviceContext->PSSetShaderResources(0, 1, (ID3D11ShaderResourceView**)&(spTex->GetSRVHandle().m_pData));

	std::shared_ptr<D3D11_Shader> spPixelShader =
		std::static_pointer_cast<D3D11_Shader>(m_spRHIPixelShader);
	std::shared_ptr<D3D11_Shader> spVertexShader =
		std::static_pointer_cast<D3D11_Shader>(m_spRHIVertexShader);

	pDeviceContext->VSSetShader((ID3D11VertexShader*)spVertexShader->GetDataHandle().m_pData, 0,0);
	pDeviceContext->PSSetShader((ID3D11PixelShader*)spPixelShader->GetDataHandle().m_pData, 0,0);
	pDeviceContext->IASetInputLayout(spVertexShader->GetInputLayout());

	static float angle = 0.0f;
	//m_cbCameraData.mView =
	//	Matrix::CreateRotationY(angle) * Matrix::CreateTranslation(0,0,-2.0f);
	m_cbCameraData.mView = Matrix::CreateFromYawPitchRoll(
							   angle, 10.0f * (3.1415926535f / 180.0f), 0.0f)
		* Matrix::CreateTranslation(0.0f, 0.3f, -3.0f);
	m_cbCameraData.mView.Invert();
	// angle += 0.005f;
	if (GetAsyncKeyState('Q'))
	{
		angle += 0.005f;
	}
	if (GetAsyncKeyState('E'))
	{
		angle -= 0.005f;
	}

	m_cbCameraData.mView = m_cbCameraData.mView.Invert();
	
	m_spRHICommandList->SetConstantBuffer(7, m_spCbCamera);
	m_spCbCamera->Write(m_cbCameraData);

	m_spRHICommandList->SetConstantBuffer(1, m_spCbMaterial);

	m_cbLightData.directionalLightDir = {0, 1, 0};
	//m_cbLightData.directionalLightColor = {0.7, 1, 0.7};
	m_spRHICommandList->SetConstantBuffer(2, m_spCbLight);
	m_spCbLight->Write(m_cbLightData);
	
	DrawModelsInScene();
	
	
	//--------------------------------
	// Effekseer
	//--------------------------------
	auto effect = GetEngine()->GetSubsystemLocator().Get<Effect>();
	if (effect)
	{
		effect->SetProjectionMatrix(m_cbCameraData.mProj);
		effect->SetCameraMatrix(m_cbCameraData.mView);
		effect->OnRender();
	}

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

void Renderer::DrawSubset(const Node* _pNode, const Material& _material,
							  const Subset& _subset)
{
	if (!_pNode)
	{
		return;
	}

	// マテリアル書き込み
	m_cbMaterialData.baseColor = _material.BaseColor;
	m_spCbMaterial->Write<Cb_Material>(m_cbMaterialData);

	ID3D11DeviceContext* pDevivceContext =
		std::static_pointer_cast<D3D11_Device>(m_spRHIDevice)
			->GetDeviceContext();

	D3D11_Texture* baseColTex =
		static_cast<D3D11_Texture*>( _material.spBaseColorTexture
						? _material.spBaseColorTexture.get()
						: GetWhiteTexture().get());

	pDevivceContext->PSSetShaderResources(
		0, 1,
		(ID3D11ShaderResourceView**)&(baseColTex->GetSRVHandle().m_pData));

	// D3DDevice::GetInstance().GetDrawState()->SetTexture(D3DDevice::GetInstance().GetWhiteTex().get(),
	// 0, true);

	m_spRHICommandList->DrawIndexed(_subset.faceCount * 3,
									_subset.faceStartIdx * 3, 0);

	//static_pointer_cast<D3D11_Device>(m_spRHIDevice)->GetDeviceContext()->DrawIndexed(
	//	_subset.faceCount * 3, _subset.faceStartIdx * 3, 0);
}

void Renderer::DrawModel(const ModelWork* _pModel, const Matrix& _mWorld)
{
	if (!_pModel)
	{
		return;
	}
	for (auto&& node : _pModel->GetNodes())
	{
		m_cbUberData.mWorld = node.m_worldTransform * _mWorld;
		m_spCbUber->Write<Cb_Uber>(m_cbUberData);
		DrawMesh(&node, _pModel->GetModelData()->GetMaterials());
	}
}

void Renderer::DrawMesh(const Node*					 _pNode,
						const std::vector<Material>& _materials)
{

	if (!_pNode->IsMesh)
	{
		return;
	}

	struct ModelVertex
	{
		ModelVertex(const Vector3& _pos, const Vector2& _uv, UINT _color,
					 Vector3 _normal, Vector3 _tangent)
		{
			Pos = _pos;
			UV = _uv;
			Color = _color;
			Normal = _normal;
			Tangent = _tangent;
		}
		Vector3 Pos = {};
		Vector2 UV = {};
		UINT	Color = 0xFFFFFFFF;
		Vector3 Normal;
		Vector3 Tangent;
	};
	std::vector<ModelVertex> vertices;
	for (auto&& v : _pNode->Verteces)
	{
		vertices.push_back(ModelVertex(v.Pos, v.UV, v.Color, v.Normal, v.Tangent));
	}

	// 頂点バッファ
	std::shared_ptr<RHI_VertexBuffer> vertexBuffer =
		std::make_shared<D3D11_VertexBuffer>(m_spRHIDevice);
	vertexBuffer->Create<ModelVertex>((uint32_t) _pNode->Verteces.size(), &vertices[0]);

	m_spRHICommandList->SetVertexBuffer(0, vertexBuffer, 0);
	//D3DDevice::GetInstance().GetDeviceContext()->IASetVertexBuffers(
	//	0, 1, vertexBuffer.GetAddress(), &oneSize, &offset);

	// インデックスバッファ
	//Buffer							  indexBuffer;
	std::shared_ptr<RHI_IndexBuffer> indexBuffer =
		std::make_shared<D3D11_IndexBuffer>(m_spRHIDevice);
	indexBuffer->Create<ModelFace>((uint32_t)_pNode->Faces.size(),
								   &_pNode->Faces[0]);
	m_spRHICommandList->SetIndexBuffer(0, indexBuffer, 0);

	//D3DDevice::GetInstance().GetDeviceContext()->IASetIndexBuffer(
	//	indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// 描画
	/*D3DDevice::GetInstance().GetDeviceContext()->DrawIndexed(UINT(_pNode->Faces.size())
	* 3, 0, 0); return;*/
	for (auto&& subset : _pNode->Subsets)
	{
		DrawSubset(_pNode, _materials[subset.materialIdx], subset);
	}
}
std::shared_ptr<Model> skyModel = nullptr;
std::shared_ptr<Model> planeModel = nullptr;

void Renderer::DrawModelsInScene()
{
	auto spScripting = GetEngine()->GetSubsystemLocator().Get<Scripting>();

	m_cbUberData.mWorld = Matrix::Identity;
	m_spRHICommandList->SetConstantBuffer(8, m_spCbUber);
	m_spCbUber->Write<Cb_Uber>(m_cbUberData);

	static std::shared_ptr<Model> drumcanModel = nullptr;

	if (!drumcanModel)
	{

		drumcanModel = std::make_shared<Model>();
		if (!drumcanModel->LoadByAssimp("Assets/Models/DrumCan/DrumCan.gltf"))
		{
			assert(0 && "Model load failed...");
		}
	}

	if (!skyModel)
	{
		skyModel = std::make_shared<Model>();
		if (!skyModel->LoadByAssimp("Assets/Models/Sky/Sky.gltf"))
		{
			assert(0 && "Model load failed...");
		}
	}

	if (!planeModel)
	{
		planeModel = std::make_shared<Model>();
		//if (!planeModel->LoadByAssimp("Assets/Models/Plane/plane_100x100_stone.gltf"))
		if (!planeModel->LoadByAssimp("Assets/Models/Ground/ground.gltf"))
		{
			assert(0 && "Model load failed...");
		}
	}

	ID3D11DeviceContext* pDevivceContext =
		std::static_pointer_cast<D3D11_Device>(m_spRHIDevice)
			->GetDeviceContext();
	pDevivceContext->IASetPrimitiveTopology(
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// render ground
	{
		ModelWork modelWork;
		modelWork.Set(planeModel);
		DrawModel(&modelWork, Matrix::CreateTranslation(0, -1,0));
	}
	// render sky
	{
		ModelWork modelWork;
		modelWork.Set(skyModel);
		DrawModel(&modelWork, Matrix::CreateScale(1, 1, 1));
	}

	// render gameobjects
	for (auto go : spScripting->GetGameObjects())
	{
		if (!go->GetEnabled())
		{
			continue;
		}

		Vector3 pos;
		auto	transform = go->GetTransform();
		for (auto spProp :
			 transform->GetClassInstance()->m_pClassType->spPropertyInfos)
		{
			if (spProp->name == "Position")
			{
				Vector3 ret;
				spProp->GetValue(transform->GetClassInstance()->m_pInstance,
								 &ret);
				// if (ret != nullptr)
				{
					pos = ret;
					// std::cout << pos.x << ", " << pos.y << std::endl;
				}
				break;
			}
		}

		{
			ModelWork modelWork;
			modelWork.Set(drumcanModel);
			DrawModel(&modelWork, Matrix::CreateTranslation(pos));
		}
	}
}
} // namespace Miyadaiku