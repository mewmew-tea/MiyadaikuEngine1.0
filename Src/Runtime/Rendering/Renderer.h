#pragma once
#include "../Core/Subsystem.h"
#include "./CommonConstantBuffer.h"

namespace Miyadaiku
{
// 前方宣言
class RHI_Device;
class RHI_SwapChain;
class RHI_Shader;
class RHI_CommandList;
class RHI_Texture;

class Model;
class ModelWork;
class Node;
class Material;
class Subset;

struct RHI_RasterizerState;
struct RHI_SamplerState;

class RHI_ConstantBuffer;

class Renderer final : public Subsystem
{
public:
	~Renderer();

	void OnAwake() override;

	void OnShutdown() override;

	void Tick();

	void Present();

	std::shared_ptr<RHI_Device> GetRHIDevice()
	{
		return m_spRHIDevice;
	}

	std::shared_ptr<RHI_Texture> GetWhiteTexture()
	{
		return m_spWhiteTexure;
	}
	std::shared_ptr<RHI_Texture> GetNormalTexture()
	{
		return m_spNormalTexure;
	}

	const Matrix& GetViewMatrix()
	{
		return m_cbCameraData.mView;
	}
	const Matrix& GetProjectionMatrix()
	{
		return m_cbCameraData.mProj;
	}

private:
	void DrawModelsInScene();

	void DrawModel(const ModelWork* _pModel, const Matrix& _mWorld);
	void DrawMesh(const Node* _pNode, const std::vector<Material>& _materials);

	void DrawSubset(const Node* _pNode, const Material& _material,
					const Subset& _subset);
	std::shared_ptr<RHI_Device>		 m_spRHIDevice = nullptr;
	std::shared_ptr<RHI_SwapChain>	 m_spRHISwapChain = nullptr;
	std::shared_ptr<RHI_CommandList> m_spRHICommandList = nullptr;

	// TODO:
	// シェーダはどこか（RHI_CommandListとか(RHI_?)RenderingPipelineとか）に移動
	// アイデアとして、RenderingPipelineの概念はUnityに近い感じ。RenderPassの組み合わせとか？

	std::shared_ptr<RHI_Shader> m_spRHIVertexShader = nullptr;
	std::shared_ptr<RHI_Shader> m_spRHIPixelShader = nullptr;


	std::shared_ptr<RHI_RasterizerState> m_spRHIRasterizerState = nullptr;
	std::shared_ptr<RHI_SamplerState> m_spRHISamplerState = nullptr;

	// Constant buffers
	// TODO: class to combine data and CBs
	Cb_Camera							m_cbCameraData;
	std::shared_ptr<RHI_ConstantBuffer> m_spCbCamera = nullptr;
	Cb_Uber								m_cbUberData;
	std::shared_ptr<RHI_ConstantBuffer> m_spCbUber = nullptr;
	Cb_Material							m_cbMaterialData;
	std::shared_ptr<RHI_ConstantBuffer> m_spCbMaterial = nullptr;
	Cb_Light							m_cbLightData;
	std::shared_ptr<RHI_ConstantBuffer> m_spCbLight = nullptr;

	// default textures
	std::shared_ptr<RHI_Texture> m_spWhiteTexure = nullptr;
	std::shared_ptr<RHI_Texture> m_spNormalTexure = nullptr;
};
} // namespace Miyadaiku