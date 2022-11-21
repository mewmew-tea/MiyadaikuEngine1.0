#pragma once
#include <string>
#include <memory>
#include <array>
#include <vector>
#include <filesystem>
#include <map>
#include "../Core/Math/Vector2.h"
#include "../Core/Math/Vector3.h"
#include "../Core/Math/Vector4.h"
#include "../Core/Math/Matrix.h"
#include "../Core/Math/Quaternion.h"

class aiScene;
class aiNode;
namespace Miyadaiku
{
class RHI_Texture;

struct Material
{
	// マテリアル名
	std::string Name = "";

	// マテリアル全体における値
	Vector4 BaseColor = {1, 1, 1, 1};
	float Metallic = 0.0f;	// 金属性のスケーリング係数
	float Roughness = 1.0f; // 粗さのスケーリング係数
	Vector3 Emissive = {
		0, 0, 0}; // 自己発光のスケーリング係数(RGB)

	// マテリアルテクスチャのファイル名
	std::string baseColorTextureName = "";
	std::string normalTextureName = "";
	std::string metallicRoughnessTexureName = "";
	std::string emissiveTexureName = "";

	// マテリアルテクスチャ情報
	std::shared_ptr<RHI_Texture> spBaseColorTexture = nullptr;
	std::shared_ptr<RHI_Texture> spNormalColorTexture = nullptr;
	std::shared_ptr<RHI_Texture> spMetallicRoughnessTexure = nullptr;
	std::shared_ptr<RHI_Texture> spEmissiveTexure = nullptr;
};

struct Subset
{
	uint32_t materialIdx = 0;
	uint32_t faceStartIdx = 0;
	uint32_t faceCount = 0;
};

struct ModelVertex
{
	Vector3				 Pos = {};
	Vector3				 Normal;  // 法線
	Vector3				 Tangent; // 接線
	Vector2				 UV = {};
	uint32_t			 Color = 0xFFFFFFFF;
	std::array<short, 4> SkinIndexList;	 // スキニングIndexリスト
	std::array<float, 4> SkinWeightList; // スキニングウェイトリスト
};

struct ModelFace
{
	uint32_t idx[3];
};

struct Node
{
	std::string Name;
	bool		IsMesh = false;

	std::vector<ModelVertex> Verteces;
	std::vector<ModelFace>	 Faces;
	std::vector<Subset>		 Subsets;

	int m_nodeIndex = -1;		 // 先頭から何番目のノードか？
	int m_parent = -1;			 // 親インデックス
	std::vector<int> m_children; // 子供へのインデックスリスト

	Matrix m_localTransform; // 直属の親ボーンからの行列
	Matrix m_worldTransform;	 // 原点からの行列
	Matrix m_inverseBindMatrix; // 原点からの逆行列
};

struct ModelAnimation
{
	struct Node
	{
		struct RotateKey
		{
			float							Time = 0.0f;
			Quaternion Quat;

			//bool Interpolation(Matrix& mOut,
			//				   const RotateKey& next, float progressTime) const;
		};
		struct TransKey
		{
			float						 Time = 0.0f;
			Vector3 vPos;

			//bool Interpolation(Matrix& mOut,
			//				   const TransKey& next, float progressTime) const;
		};

		int					   ModelNodeIndex = -1;
		std::vector<RotateKey> Rotations;
		std::vector<TransKey>  Translations;
	};

	std::string		  Name = "";
	float			  Length = 0.0f;
	std::vector<Node> Nodes;
};

class Model
{
public:
	bool LoadByAssimp(const std::string& _filename);
	bool LoadFromMDM(const std::string& _filepath);
	bool SaveToMDM(const std::string& _filepath);

	// メッシュのロードおかしいからテスト用
	bool LoadMeshFace(const std::string& _filepath);
	bool SaveMeshFace(const std::string& _filepath);

	//bool							Create(const KdGLTFModel* _pModelData);
	bool							Create(const aiScene* _pScene);
	const std::vector<ModelVertex>& GetVerteces(uint32_t _nodeIdx) const
	{
		return m_nodes[_nodeIdx].Verteces;
	}
	const std::vector<ModelFace>& GetFaces(uint32_t _nodeIdx) const
	{
		return m_nodes[_nodeIdx].Faces;
	}
	const std::vector<Node>& GetNodes() const
	{
		return m_nodes;
	}
	const std::vector<Material>& GetMaterials() const
	{
		return m_materials;
	}
	const std::vector<int>& GetRootNodeIndices() const
	{
		return m_rootNodeIndices;
	}

	const std::vector<ModelAnimation>& GetAnimations() const
	{
		return m_animations;
	}

private:
	void ProcessNode(aiNode* _pNode, const aiScene* _pScene);
	// Mesh ProcessMesh(aiMesh* _pMesh, const aiScene* _pScene);

	std::vector<Node>	  m_nodes;
	std::filesystem::path m_filePath;
	std::vector<Material> m_materials;
	// ルートノード達のIndex
	std::vector<int>	  m_rootNodeIndices;

	std::vector<ModelAnimation> m_animations;

	std::map<const void*, int> m_aiNodeIndexMap;
};

class ModelWork
{
public:
	void Set(std::shared_ptr<Model>& _pModel);

	void CalcWorldMatrixes();

	const std::shared_ptr<Model> GetModelData() const
	{
		return m_spModelData;
	}
	const std::vector<Node>& GetNodes() const
	{
		return m_coppiedNodes;
	}
	std::vector<Node>& WorkNodes()
	{
		return m_coppiedNodes;
	}

	//void Animation(float speed = 0.01f);

private:
	std::shared_ptr<Model> m_spModelData;
	// DirectX::SimpleMath::Matrix	m_mOrigin;
	std::vector<Node>	   m_coppiedNodes;
};
}