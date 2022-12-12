#pragma warning(push)
#pragma warning(disable : 4100)
#pragma warning(disable : 26495)
#define NOMINMAX
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#pragma warning(pop)

#include "Model.h"

#include "RHI/RHI_Texture.h"
#include "RHI/D3D11/D3D11_Texture.h"

#include "../Core/Engine.h"
#include "Renderer.h"

#include <fstream>
#include <functional>

namespace Miyadaiku
{
//#include "../../../Libraries/KdGltfLoader/KdGLTFLoader.h"

//bool Model::LoadFromGLTF(const std::string& _filename)
//{
//	if (_filename.empty())
//	{
//		return false;
//	}
//
//	m_filePath = _filename;
//	std::shared_ptr<KdGLTFModel> gltf = KdLoadGLTFModel(_filename);
//
//	return Create(gltf.get());
//}

bool Model::LoadByAssimp(const std::string& _filename)
{
	Assimp::Importer importer;

	int flag = 0;
	flag |= aiProcess_Triangulate;
	flag |= aiProcess_PreTransformVertices;
	flag |= aiProcess_CalcTangentSpace;
	flag |= aiProcess_GenSmoothNormals;
	flag |= aiProcess_GenUVCoords;
	flag |= aiProcess_RemoveRedundantMaterials;
	flag |= aiProcess_OptimizeMeshes;
	//flag |= aiProcess_ConvertToLeftHanded;
	const aiScene* pScene = importer.ReadFile(_filename,
											  /*aiProcess_Triangulate |
											  aiProcess_SortByPType |
											  aiProcess_ConvertToLeftHanded*/
											  flag);

	if (pScene == nullptr)
		return false;

	m_filePath = _filename;

	Create(pScene);
	// processNode(pScene->mRootNode, pScene);

	//
	pScene = nullptr;
	return true;
}

// 最新のバージョン
std::uint8_t MDM_CURRENT_VERSION = 1;

// ヘッダ部分
struct MDMHeader
{
	char		  fileType[3];	// バージョン
	std::uint8_t  version;		// バージョン
	std::uint16_t numNodes;		// メッシュパーツの数
	std::uint16_t numMaterials; // マテリアルの数
	std::uint8_t  RESERVED[8];	// 予約領域
};

// ノード情報のヘッダ
struct MDMNodeHeader
{
	std::uint32_t numVerteces; // 頂点の数
	std::uint8_t  numFaces;	   // 面の数
	std::uint8_t  numSubsets;  // サブセットの数
	std::uint8_t  numChildren; // 子ノードの数
	std::uint8_t  BLANK[1];	   // 空き領域
};

// 頂点情報
struct MDMVertex
{
	float		  pos[3];	  // 頂点座標
	float		  normal[3];  // 法線
	float		  tangent[3]; // 接線
	float		  uv[2];	  // UV座標
	std::uint32_t color;	  // カラー
	std::int16_t  indices[4]; // スキニングIndexリスト
	float		  weights[4]; // スキニングウェイトリスト
};

// サブセット情報
struct MDMSubset
{
	uint16_t materialIdx = 0;
	uint32_t faceStartIdx = 0;
	uint32_t faceCount = 0;
};

// マテリアルの値
struct MDMMaterialValue
{
	float baseColor[4] = {1, 1, 1, 1};
	float metalic = 0.0f;
	float roughness = 1.0f;
	float emissive[3] = {0, 0, 0};
};

// MDMファイルの文字列書式に合わせて、文字列を読み込む
std::string ReadMDMString(FILE* fp)
{
	std::string	  str;
	// 文字列の長さ取得
	std::uint32_t strLength = 0;
	fread(&strLength, sizeof(strLength), 1, fp);

	if (strLength > 0)
	{
		// 終端文字を含めて、多めに確保
		char* buf = nullptr;
		buf = reinterpret_cast<char*>(malloc(strLength + 1));
		if (buf)
		{
			fread_s(buf, strLength + 1, strLength + 1, 1, fp);
			str = buf;
			free(buf);
		}
	}

	return str;
}

// MDMファイルの文字列書式に合わせて、文字列を読み込む
std::string ReadMDMString(std::fstream& fs)
{
	std::string	  str;
	// 文字列の長さ取得
	std::uint32_t strLength = 0;
	fs.read((char*)&strLength, sizeof(strLength));

	if (strLength > 0)
	{
		// 終端文字を含めて、多めに確保
		char* buf = nullptr;
		buf = reinterpret_cast<char*>(malloc(strLength + 1));
		if (buf)
		{
			fs.read((char*)buf, strLength + 1);
			str = buf;
			free(buf);
		}
	}

	return str;
}

// MDMファイルの文字列書式に合わせて、文字列を書き込む
bool WriteMDMString(FILE* fp, std::string_view str)
{
	// 文字列の長さ書き込み
	std::uint32_t strLength = static_cast<std::uint32_t>(str.length());
	if (fwrite(&strLength, sizeof(strLength), 1, fp) != 1)
	{
		return false;
	}
	if (strLength > 0)
	{
		// 終端文字を含めて書き込み
		std::string s{str};
		if (fwrite(s.c_str(), strLength + 1, 1, fp) != 1)
		{
			return false;
		}
	}
	return true;
}

// MDMファイルの文字列書式に合わせて、文字列を書き込む
bool WriteMDMString(std::fstream& fs, std::string_view str)
{
	// 文字列の長さ書き込み
	std::uint32_t strLength = static_cast<std::uint32_t>(str.length());
	if (!fs.write((char*)&strLength, sizeof(strLength)))
	{
		return false;
	}
	if (strLength > 0)
	{
		// 終端文字を含めて書き込み
		std::string s{str};
		if (!fs.write(s.c_str(), strLength + 1))
		{
			return false;
		}
	}
	return true;
}

bool Model::LoadFromMDM(const std::string& _filepath)
{
	auto spRenderer = GetEngine()->GetSubsystemLocator().Get<Renderer>();

	m_filePath = _filepath;

	std::fstream fs;
	fs.open(_filepath.c_str(), std::ios::binary | std::ios::in);
	if (!fs)
	{
		assert(0 && "MDMファイルを開けませんでした。");
		fs.close();
		return false;
	}

	// ヘッダ
	MDMHeader header = {};
	fs.read((char*)&header, sizeof(header));
	if (!(header.fileType[0] == 'M' && header.fileType[1] == 'D'
		  && header.fileType[2] == 'M'))
	{
		assert(0 && "MDMファイルフォーマットではありません。");
	}
	if (header.version != MDM_CURRENT_VERSION)
	{
		assert(0 && "このMDMファイルのバージョンは未対応です。");
	}

	// ノード
	m_nodes.reserve(header.numNodes);
	for (int i = 0; i < header.numNodes; ++i)
	{
		Node newNode;

		// ノードヘッダ
		MDMNodeHeader nodeHeader = {};
		fs.read((char*)&nodeHeader, sizeof(MDMNodeHeader));

		// 名前
		newNode.Name = ReadMDMString(fs);
		// メッシュか？
		char isMesh;
		fs.read((char*)&isMesh, sizeof(char));
		newNode.IsMesh = (isMesh == 1);

		// 頂点情報
		std::vector<MDMVertex> verteces;
		if (nodeHeader.numVerteces > 0)
		{
			verteces.resize(nodeHeader.numVerteces);
			// verteces.push_back(MDMVertex());
			newNode.Verteces.reserve(nodeHeader.numVerteces);
			fs.read((char*)&verteces[0],
					sizeof(MDMVertex) * nodeHeader.numVerteces);
			for (auto&& vertex : verteces)
			{
				ModelVertex newVewtex = {};
				newVewtex.Pos = {vertex.pos[0], vertex.pos[1], vertex.pos[2]};
				newVewtex.Normal = {vertex.normal[0], vertex.normal[1],
									vertex.normal[2]};
				newVewtex.Tangent = {vertex.tangent[0], vertex.tangent[1],
									 vertex.tangent[2]};
				newVewtex.UV = {vertex.uv[0], vertex.uv[1]};
				newVewtex.Color = static_cast<uint32_t>(vertex.color);
				newVewtex.SkinIndexList[0] =
					static_cast<short>(vertex.indices[0]);
				newVewtex.SkinIndexList[1] =
					static_cast<short>(vertex.indices[1]);
				newVewtex.SkinIndexList[2] =
					static_cast<short>(vertex.indices[2]);
				newVewtex.SkinIndexList[3] =
					static_cast<short>(vertex.indices[3]);
				newVewtex.SkinWeightList[0] = vertex.weights[0];
				newVewtex.SkinWeightList[1] = vertex.weights[1];
				newVewtex.SkinWeightList[2] = vertex.weights[2];
				newVewtex.SkinWeightList[3] = vertex.weights[3];
				newNode.Verteces.push_back(newVewtex);
			}
		}

		// 面
		std::vector<std::uint32_t> faces;
		if (nodeHeader.numFaces > 0)
		{
			faces.resize(nodeHeader.numFaces * 3);
			// faces.push_back(std::uint32_t());
			newNode.Faces.reserve(nodeHeader.numFaces);
			fs.read((char*)&faces[0],
					sizeof(std::uint32_t) * nodeHeader.numFaces * 3);
			// fread_s(&faces[0], sizeof(std::uint32_t) * faces.size(),
			// sizeof(std::uint32_t) * faces.size(), 1, fp);
			for (int j = 0; j < nodeHeader.numFaces * 3; j += 3)
			{
				ModelFace newFace = {};

				// fread_s(&faces[j], sizeof(std::uint32_t),
				// sizeof(std::uint32_t), 1, fp);
				newFace.idx[0] = static_cast<uint32_t>(faces[j]);
				// fread_s(&faces[j + 1], sizeof(std::uint32_t),
				// sizeof(std::uint32_t), 1, fp);
				newFace.idx[1] = static_cast<uint32_t>(faces[j + 1]);
				// fread_s(&faces[j + 2], sizeof(std::uint32_t),
				// sizeof(std::uint32_t), 1, fp);
				newFace.idx[2] = static_cast<uint32_t>(faces[j + 2]);

				newNode.Faces.push_back(newFace);
			}
		}

		// サブセット
		std::vector<MDMSubset> subsets;
		if (nodeHeader.numSubsets > 0)
		{
			subsets.resize(nodeHeader.numSubsets);
			// subsets.push_back(MDMSubset());
			newNode.Subsets.reserve(nodeHeader.numSubsets);
			fs.read((char*)&subsets[0],
					sizeof(MDMSubset) * nodeHeader.numSubsets);
			for (auto&& subset : subsets)
			{
				Subset newSubset = {};
				newSubset.materialIdx = static_cast<uint32_t>(subset.materialIdx);
				newSubset.faceStartIdx = static_cast<uint32_t>(subset.faceStartIdx);
				newSubset.faceCount = static_cast<uint32_t>(subset.faceCount);
				newNode.Subsets.push_back(newSubset);
			}
		}

		// ノードのインデックス
		std::int16_t nodeIdx;
		fs.read((char*)&nodeIdx, sizeof(std::int16_t));
		newNode.m_nodeIndex = static_cast<int>(nodeIdx);

		// 親のノードインデックス
		std::int16_t parentNodeIdx;
		fs.read((char*)&parentNodeIdx, sizeof(std::int16_t));
		newNode.m_parent = static_cast<int>(parentNodeIdx);

		// 子供へのインデックスリスト
		std::vector<std::int16_t> children;
		if (nodeHeader.numChildren > 0)
		{
			children.resize(nodeHeader.numChildren);
			fs.read((char*)&children[0],
					sizeof(std::int16_t) * nodeHeader.numChildren);
			for (auto&& child : children)
			{
				newNode.m_children.push_back(int(child));
			}
		}

		// ローカル：親からの行列
		fs.read((char*)&newNode.m_worldTransform.m11, sizeof(float) * 16);
		// ワールド：原点からの行列
		fs.read((char*)&newNode.m_localTransform.m11, sizeof(float) * 16);
		// 原点からの逆行列
		fs.read((char*)&newNode.m_inverseBindMatrix.m11, sizeof(float) * 16);

		m_nodes.push_back(newNode);
	}

	// マテリアル
	m_materials.reserve(header.numMaterials);
	for (int i = 0; i < header.numMaterials; ++i)
	{
		Material newMaterial;

		// マテリアル名
		newMaterial.Name = ReadMDMString(fs);

		// 各種マテリアル値
		MDMMaterialValue matValue = {};
		fs.read((char*)&matValue, sizeof(MDMMaterialValue));

		// ベースカラー
		newMaterial.BaseColor = {matValue.baseColor[0], matValue.baseColor[1],
								 matValue.baseColor[2], matValue.baseColor[3]};
		// メタリック
		newMaterial.Metallic = matValue.metalic;
		newMaterial.Roughness = matValue.roughness;
		// エミッシブ
		newMaterial.Emissive = {matValue.emissive[1], matValue.emissive[1],
								matValue.emissive[2]};

		// マテリアルのテクスチャ
		std::filesystem::path texturePath = m_filePath;
		// ベースカラー
		newMaterial.baseColorTextureName = ReadMDMString(fs);
		texturePath.replace_filename(newMaterial.baseColorTextureName);
		if (!newMaterial.baseColorTextureName.empty())
		{
			newMaterial.spBaseColorTexture = std::make_shared<D3D11_Texture>(spRenderer->GetRHIDevice());
			if (!newMaterial.spBaseColorTexture)
			{
				return false;
			}
			if (!newMaterial.spBaseColorTexture->Create(texturePath.string()))
			{
				newMaterial.spBaseColorTexture =
					spRenderer->GetWhiteTexture();
			}
		}
		// ノーマル
		newMaterial.normalTextureName = ReadMDMString(fs);
		texturePath.replace_filename(newMaterial.normalTextureName);
		if (!newMaterial.normalTextureName.empty())
		{
			newMaterial.spNormalColorTexture = std::make_shared<D3D11_Texture>(spRenderer->GetRHIDevice());
			if (!newMaterial.spNormalColorTexture)
			{
				return false;
			}
			if (!newMaterial.spNormalColorTexture->Create(texturePath.string()))
			{
				newMaterial.spNormalColorTexture =
					spRenderer->GetNormalTexture();
			}
		}
		// メタリック・ラフネス
		newMaterial.metallicRoughnessTexureName = ReadMDMString(fs);
		texturePath.replace_filename(newMaterial.metallicRoughnessTexureName);
		if (!newMaterial.metallicRoughnessTexureName.empty())
		{
			newMaterial.spMetallicRoughnessTexure = std::make_shared<D3D11_Texture>(spRenderer->GetRHIDevice());
			if (!newMaterial.spMetallicRoughnessTexure)
			{
				return false;
			}
			if (!newMaterial.spMetallicRoughnessTexure->Create(
					texturePath.string()))
			{
				newMaterial.spMetallicRoughnessTexure =
					spRenderer->GetWhiteTexture();
			}
		}
		// エミッシブ
		newMaterial.emissiveTexureName = ReadMDMString(fs);
		texturePath.replace_filename(newMaterial.emissiveTexureName);
		if (!newMaterial.emissiveTexureName.empty())
		{
			newMaterial.spEmissiveTexure = std::make_shared<D3D11_Texture>(spRenderer->GetRHIDevice());
			if (!newMaterial.spEmissiveTexure)
			{
				return false;
			}
			if (!newMaterial.spEmissiveTexure->Create(texturePath.string()))
			{
				newMaterial.spEmissiveTexure = spRenderer->GetWhiteTexture();

			}
		}

		m_materials.push_back(newMaterial);
	}

	fs.close();
	return true;
}

bool Model::SaveToMDM(const std::string& _filepath)
{
	std::fstream fs;
	fs.open(_filepath.c_str(), std::ios::binary | std::ios::out);
	if (!fs)
	{
		assert(0 && "MDMファイルを開けませんでした。");
		fs.close();
		return false;
	}

	// ヘッダ
	MDMHeader header = {};
	header.fileType[0] = 'M';
	header.fileType[1] = 'D';
	header.fileType[2] = 'M';
	header.version = MDM_CURRENT_VERSION;
	header.numNodes = (uint16_t)m_nodes.size();
	header.numMaterials = (uint16_t)m_materials.size();
	if (!fs.write((char*)&header, sizeof(MDMHeader)))
	{
		assert(0 && "MDMヘッダ書き込み失敗");
		fs.close();
		return false;
	}

	// ノード
	//const uint32_t numNodes = (uint32_t)m_nodes.size();
	for (auto&& node : m_nodes)
	{
		// ノードヘッダ
		MDMNodeHeader nodeHeader;
		nodeHeader.numVerteces =
			static_cast<std::uint32_t>(node.Verteces.size());
		nodeHeader.numFaces = static_cast<std::uint8_t>(node.Faces.size());
		nodeHeader.numSubsets = static_cast<std::uint8_t>(node.Subsets.size());
		nodeHeader.numChildren =
			static_cast<std::uint8_t>(node.m_children.size());
		if (!fs.write((char*)&nodeHeader, sizeof(MDMNodeHeader)))
		{
			fs.close();
			return false;
		}

		// 名前
		if (!WriteMDMString(fs, node.Name))
		{
			fs.close();
			return false;
		}
		// メッシュか？
		char isMesh = node.IsMesh ? 1 : 0;
		if (!fs.write((char*)&isMesh, sizeof(isMesh)))
		{
			fs.close();
			return false;
		}
		// 頂点情報
		std::vector<MDMVertex> verteces;
		verteces.reserve(node.Verteces.size());
		for (auto&& vertex : node.Verteces)
		{
			MDMVertex newVewtex = {};
			newVewtex.pos[0] = vertex.Pos.x;
			newVewtex.pos[1] = vertex.Pos.y;
			newVewtex.pos[2] = vertex.Pos.z;
			newVewtex.normal[0] = vertex.Normal.x;
			newVewtex.normal[1] = vertex.Normal.y;
			newVewtex.normal[2] = vertex.Normal.z;
			newVewtex.tangent[0] = vertex.Tangent.x;
			newVewtex.tangent[1] = vertex.Tangent.y;
			newVewtex.tangent[2] = vertex.Tangent.z;
			newVewtex.uv[0] = vertex.UV.x;
			newVewtex.uv[1] = vertex.UV.y;
			newVewtex.color = static_cast<std::uint32_t>(vertex.Color);
			newVewtex.indices[0] =
				static_cast<std::int16_t>(vertex.SkinIndexList[0]);
			newVewtex.indices[1] =
				static_cast<std::int16_t>(vertex.SkinIndexList[1]);
			newVewtex.indices[2] =
				static_cast<std::int16_t>(vertex.SkinIndexList[2]);
			newVewtex.indices[3] =
				static_cast<std::int16_t>(vertex.SkinIndexList[3]);
			newVewtex.weights[0] = vertex.SkinWeightList[0];
			newVewtex.weights[1] = vertex.SkinWeightList[1];
			newVewtex.weights[2] = vertex.SkinWeightList[2];
			newVewtex.weights[3] = vertex.SkinWeightList[3];
			verteces.push_back(newVewtex);
		}
		if (verteces.size() > 0)
		{
			if (!fs.write((char*)&verteces[0],
						  sizeof(MDMVertex) * verteces.size()))
			{
				fs.close();
				return false;
			}
		}

		// 面
		std::vector<std::uint32_t> faces;
		faces.reserve(node.Faces.size() * 3);
		for (auto&& face : node.Faces)
		{
			faces.push_back(static_cast<std::uint32_t>(face.idx[0]));
			faces.push_back(static_cast<std::uint32_t>(face.idx[1]));
			faces.push_back(static_cast<std::uint32_t>(face.idx[2]));
		}
		//int s = sizeof(std::uint32_t) * (uint32_t)faces.size();
		if (faces.size() > 0)
		{
			/*if (fwrite(&faces[0], sizeof(std::uint32_t) * faces.size(), 1, fp)
			!= 1) { fclose(fp); return false;
			}*/

			//std::uint32_t* address = &faces[0];
			for (int f = 0; f < faces.size(); ++f)
			{

				if (!fs.write((char*)&faces[f], sizeof(std::uint32_t)))
				{
					fs.close();
					return false;
				}

				// OutputDebugStringA( (std::to_string(*(address + f)) + ",
				// ").c_str());
			}
			// OutputDebugStringA("\n");
		}

		// サブセット
		std::vector<MDMSubset> subsets;
		subsets.reserve(node.Subsets.size());
		for (auto&& subset : node.Subsets)
		{
			MDMSubset newSubset = {};
			newSubset.materialIdx =
				static_cast<std::uint16_t>(subset.materialIdx);
			newSubset.faceStartIdx =
				static_cast<std::uint16_t>(subset.faceStartIdx);
			newSubset.faceCount = static_cast<std::uint16_t>(subset.faceCount);
			subsets.push_back(newSubset);
		}
		if (subsets.size() > 0)
		{
			if (!fs.write((char*)&subsets[0],
						  sizeof(MDMSubset) * subsets.size()))
			{
				fs.close();
				return false;
			}
		}

		// ノードのインデックス
		std::int16_t nodeIdx = static_cast<std::int16_t>(node.m_nodeIndex);
		if (!fs.write((char*)&nodeIdx, sizeof(std::int16_t)))
		{
			fs.close();
			return false;
		}

		// 親のノードインデックス
		std::int16_t parentNodeIdx = static_cast<std::int16_t>(node.m_parent);
		if (!fs.write((char*)&parentNodeIdx, sizeof(std::int16_t)))
		{
			fs.close();
			return false;
		}

		// 子供へのインデックスリスト
		std::vector<std::int16_t> children;
		children.reserve(node.m_children.size());
		for (auto&& child : node.m_children)
		{
			children.push_back((uint16_t)child);
		}
		if (children.size() > 0)
		{
			if (!fs.write((char*)&children[0],
						  sizeof(std::int16_t) * children.size()))
			{
				fs.close();
				return false;
			}
		}

		// ローカル：親からの行列
		if (!fs.write((char*)&node.m_localTransform.m11, sizeof(float) * 16))
		{
			fs.close();
			return false;
		}
		// ワールド：原点からの行列
		if (!fs.write((char*)&node.m_worldTransform.m11, sizeof(float) * 16))
		{
			fs.close();
			return false;
		}
		// 原点からの逆行列
		if (!fs.write((char*)&node.m_inverseBindMatrix.m11, sizeof(float) * 16))
		{
			fs.close();
			return false;
		}
	}

	// マテリアル
	for (auto&& material : m_materials)
	{
		// マテリアル名
		if (!WriteMDMString(fs, material.Name))
		{
			fs.close();
			return false;
		}

		MDMMaterialValue matValue = {};
		// ベースカラー
		matValue.baseColor[0] = material.BaseColor.x;
		matValue.baseColor[1] = material.BaseColor.y;
		matValue.baseColor[2] = material.BaseColor.z;
		matValue.baseColor[3] = material.BaseColor.w;
		// メタリック
		matValue.metalic = material.Metallic;
		// ラフネス
		matValue.roughness = material.Roughness;
		// エミッシブ
		matValue.emissive[0] = material.Emissive.x;
		matValue.emissive[1] = material.Emissive.y;
		matValue.emissive[2] = material.Emissive.z;
		if (!fs.write((char*)&matValue, sizeof(MDMMaterialValue)))
		{
			fs.close();
			return false;
		}
		// マテリアルのテクスチャ
		if (!WriteMDMString(fs, material.baseColorTextureName))
		{
			fs.close();
			return false;
		}
		if (!WriteMDMString(fs, material.normalTextureName))
		{
			fs.close();
			return false;
		}
		if (!WriteMDMString(fs, material.metallicRoughnessTexureName))
		{
			fs.close();
			return false;
		}
		if (!WriteMDMString(fs, material.emissiveTexureName))
		{
			fs.close();
			return false;
		}
	}

	fs.close();
	return true;
}

bool Model::LoadMeshFace(const std::string& _filepath)
{

	m_filePath = _filepath;

	std::fstream fs;
	fs.open(_filepath.c_str(), std::ios::binary | std::ios::in);
	if (!fs)
	{
		assert(0 && "MDMファイルを開けませんでした。");
		fs.close();
		return false;
	}

	// ヘッダ
	std::uint16_t numNodes = 0;
	fs.read((char*)&numNodes, sizeof(numNodes));

	// ノード
	m_nodes.reserve(numNodes);
	for (int i = 0; i < numNodes; ++i)
	{
		Node		 newNode = {};
		std::uint8_t numFaces = 0;
		fs.read((char*)&numFaces, sizeof(numFaces));

		// 面
		std::vector<std::uint32_t> faces;
		if (numFaces > 0)
		{
			faces.resize(numFaces * 3);
			// faces.push_back(std::uint32_t());
			newNode.Faces.reserve(numFaces);
			fs.read((char*)&faces[0], sizeof(std::uint32_t) * numFaces * 3);
			// fread_s(&faces[0], sizeof(std::uint32_t) * faces.size(),
			// sizeof(std::uint32_t) * faces.size(), 1, fp);
			for (int j = 0; j < numFaces * 3; j += 3)
			{
				ModelFace newFace = {};

				// fread_s(&faces[j], sizeof(std::uint32_t),
				// sizeof(std::uint32_t), 1, fp);
				newFace.idx[0] = static_cast<uint32_t>(faces[j]);
				// fread_s(&faces[j + 1], sizeof(std::uint32_t),
				// sizeof(std::uint32_t), 1, fp);
				newFace.idx[1] = static_cast<uint32_t>(faces[j + 1]);
				// fread_s(&faces[j + 2], sizeof(std::uint32_t),
				// sizeof(std::uint32_t), 1, fp);
				newFace.idx[2] = static_cast<uint32_t>(faces[j + 2]);

				newNode.Faces.push_back(newFace);
			}
		}
		m_nodes.push_back(newNode);
	}
	fs.close();
	return true;
}

bool Model::SaveMeshFace(const std::string& _filepath)
{
	std::fstream fs;
	fs.open(_filepath.c_str(), std::ios::binary | std::ios::out);
	if (!fs)
	{
		assert(0 && "MDMファイルを開けませんでした。");
		fs.close();
		return false;
	}
	// ノード
	std::uint16_t numNodes = (uint16_t)m_nodes.size();
	if (!fs.write((char*)&numNodes, sizeof(numNodes)))
	{
		fs.close();
		return false;
	}
	for (auto&& node : m_nodes)
	{
		// 面の数
		std::uint8_t numFaces = static_cast<std::uint8_t>(node.Faces.size());
		if (!fs.write((char*)&numFaces, sizeof(numFaces)))
		{
			fs.close();
			return false;
		}

		// 面
		std::vector<std::uint32_t> faces;
		faces.reserve(node.Faces.size() * 3);
		for (auto&& face : node.Faces)
		{
			faces.push_back(static_cast<std::uint32_t>(face.idx[0]));
			faces.push_back(static_cast<std::uint32_t>(face.idx[1]));
			faces.push_back(static_cast<std::uint32_t>(face.idx[2]));
		}
		//int s = sizeof(std::uint32_t) * faces.size();
		if (faces.size() > 0)
		{

			//std::uint32_t* address = &faces[0];
			for (int f = 0; f < faces.size(); ++f)
			{
				if (!fs.write((char*)&faces[f], sizeof(std::uint32_t)))
				{
					fs.close();
					return false;
				}
			}
		}
	}

	fs.close();
	return true;
}
//
//bool Model::Create(const KdGLTFModel* _pModelData)
//{
//	// ルートノード達
//	m_rootNodeIndices = _pModelData->m_rootNodeIndices;
//
//	for (const KdGLTFNode& node : _pModelData->m_nodes)
//	{
//		Node newNode;
//		newNode.Name = node.m_name;
//		newNode.IsMesh = node.m_isMesh;
//		newNode.m_parent = node.m_parent;
//		newNode.m_worldTransform = node.m_worldTransform;
//		// newNode.m_worldTransform = node.m_localTransform;
//		newNode.m_localTransform = node.m_localTransform;
//		newNode.m_inverseBindMatrix = node.m_inverseBindMatrix;
//
//		newNode.m_nodeIndex = node.m_boneNodeIndex;
//
//		newNode.m_parent = node.m_parent;
//		newNode.m_children = node.m_children;
//
//		if (node.m_isMesh)
//		{
//			for (const GLTFVertex& vSrc : node.m_mesh.m_vertices)
//			{
//				ModelVertex vDest = {};
//				vDest.Pos = vSrc.m_pos;
//				vDest.UV = vSrc.m_uv;
//				vDest.Color = vSrc.m_color;
//				newNode.Verteces.push_back(vDest);
//			}
//			for (const GLTFFace& fSrc : node.m_mesh.m_faces)
//			{
//				ModelFace fDest = {};
//				fDest.idx[0] = fSrc.m_idx[0];
//				fDest.idx[1] = fSrc.m_idx[1];
//				fDest.idx[2] = fSrc.m_idx[2];
//				newNode.Faces.push_back(fDest);
//			}
//			for (auto&& sSrc : node.m_mesh.m_subsets)
//			{
//				Subset sDest = {};
//				sDest.materialIdx = sSrc.m_materialNo;
//				sDest.faceStartIdx = sSrc.m_faceStart;
//				sDest.faceCount = sSrc.m_faceCount;
//				newNode.Subsets.push_back(sDest);
//			}
//		}
//
//		m_nodes.push_back(newNode);
//	}
//
//	// マテリアル情報を取得
//	for (auto&& mat : _pModelData->m_materials)
//	{
//		// ファイルパス取得
//		Material newMat = {};
//		newMat.baseColorTextureName = mat.m_baseColorTexture;
//		std::filesystem::path texturePath = m_filePath;
//		texturePath.replace_filename(newMat.baseColorTextureName);
//
//		// テクスチャ読み込み
//		newMat.spBaseColorTexture = std::make_shared<D3D11_Texture>(spRenderer->GetRHIDevice());
//		if (!newMat.spBaseColorTexture)
//		{
//			return false;
//		}
//		newMat.spBaseColorTexture->Create(texturePath.string());
//
//		m_materials.push_back(newMat);
//	}
//
//	// アニメーション
//	for (auto&& srcAnim : _pModelData->m_animations)
//	{
//		ModelAnimation destAnim;
//		destAnim.Name = srcAnim->m_name;
//		destAnim.Length = srcAnim->m_length;
//
//		for (auto&& srcAnimNode : srcAnim->m_nodes)
//		{
//			ModelAnimation::Node destAnimNode;
//			destAnimNode.ModelNodeIndex = srcAnimNode->m_modelNodeIndex;
//			for (auto&& srcRot : srcAnimNode->m_rotations)
//			{
//				ModelAnimation::Node::RotateKey rotKey;
//				rotKey.Time = srcRot.m_time;
//				rotKey.Quat = srcRot.m_quat;
//				destAnimNode.Rotations.push_back(rotKey);
//			}
//			destAnim.Nodes.push_back(destAnimNode);
//		}
//		m_animations.push_back(destAnim);
//	}
//
//	return true;
//}

bool Model::Create(const aiScene* _pScene)
{
	auto spRenderer = GetEngine()->GetSubsystemLocator().Get<Renderer>();

	if (!_pScene)
	{
		return false;
	}

	// ノードIndexと対応するMapを作成
	{
		int							index = 0;
		std::map<const void*, int>& rNodeIndexMap = m_aiNodeIndexMap;
		std::function<void(const aiNode*, int&)> rec =
			[&rec, &rNodeIndexMap](const aiNode* _pNode, int& _idx)
		{
			// if (_pNode->mNumMeshes > 0)
			{
				rNodeIndexMap[_pNode] = _idx;
				_idx++;
			}
			for (uint32_t i = 0; i < _pNode->mNumChildren; i++)
			{

				rec(_pNode->mChildren[i], _idx);
			}
		};

		rec(_pScene->mRootNode, index);
	}

	// ノード
	ProcessNode(_pScene->mRootNode, _pScene);
	/*for (int i = 0; i < _pScene->mRootNode->mNumChildren; ++i)
	{
		ProcessNode(_pScene->mRootNode->mChildren[i], _pScene);
	}*/

	// マテリアル
	const uint32_t matNum = _pScene->mNumMaterials;
	m_materials.reserve(matNum);
	for (uint32_t i = 0; i < matNum; ++i)
	{
		aiMaterial* aiMat = _pScene->mMaterials[i];
		Material	newMat = {};
		aiString	str;

		// 名前
		newMat.Name = std::string(aiMat->GetName().C_Str());

		// 頂点カラー
		aiColor3D color;
		aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		newMat.BaseColor = {color.r, color.g, color.b, 1.0f};

		float uintVias = static_cast<float>(0xF);
		uint32_t  uiCol[3] = {static_cast<uint32_t>(color.r * uintVias),
						  static_cast<uint32_t>(color.g * uintVias),
						  static_cast<uint32_t>(color.b * uintVias)};

		//---------------------
		// テクスチャ
		//---------------------
		// ベースカラー
		auto result = aiMat->GetTexture(aiTextureType_BASE_COLOR, 0, &str);
		if (result == aiReturn::aiReturn_FAILURE)
		{
			result = aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &str);
		}
		// aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &str);
		newMat.baseColorTextureName = std::string(str.C_Str());
		std::filesystem::path texturePath = m_filePath;
		texturePath.replace_filename(newMat.baseColorTextureName);
		// テクスチャ読み込み
		if (result != aiReturn::aiReturn_FAILURE
			|| !newMat.baseColorTextureName.empty())
		{
			newMat.spBaseColorTexture = std::make_shared<D3D11_Texture>(spRenderer->GetRHIDevice());
			if (!newMat.spBaseColorTexture)
			{
				return false;
			}
			if (!newMat.spBaseColorTexture->Create(texturePath.string()))
			{
				newMat.spBaseColorTexture = spRenderer->GetWhiteTexture();

			}
		}
		else
		{
			newMat.spBaseColorTexture = spRenderer->GetWhiteTexture();
		}

		// Normal
		aiMat->GetTexture(aiTextureType_NORMALS, 0, &str);
		newMat.normalTextureName = std::string(str.C_Str());
		texturePath = m_filePath;
		texturePath.replace_filename(newMat.normalTextureName);
		// テクスチャ読み込み
		if (!newMat.normalTextureName.empty())
		{
			newMat.spNormalColorTexture = std::make_shared<D3D11_Texture>(spRenderer->GetRHIDevice());
			if (!newMat.spNormalColorTexture)
			{
				return false;
			}
			if (!newMat.spNormalColorTexture->Create(texturePath.string()))
			{
				newMat.spNormalColorTexture = spRenderer->GetNormalTexture();

			}
		}
		else
		{
			newMat.spNormalColorTexture = spRenderer->GetNormalTexture();
		}

		// Metallic, Roughness
		str = "";
		// aiMat->GetTexture(aiTextureType_METALNESS, 0, &str);
		aiMat->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &str);
		newMat.metallicRoughnessTexureName = std::string(str.C_Str());
		texturePath = m_filePath;
		texturePath.replace_filename(newMat.metallicRoughnessTexureName);
		// テクスチャ読み込み
		if (!newMat.metallicRoughnessTexureName.empty())
		{
			newMat.spMetallicRoughnessTexure = std::make_shared<D3D11_Texture>(spRenderer->GetRHIDevice());
			if (!newMat.spMetallicRoughnessTexure)
			{
				return false;
			}
			if (!newMat.spMetallicRoughnessTexure->Create(texturePath.string()))
			{
				newMat.spMetallicRoughnessTexure =
					spRenderer->GetWhiteTexture();
			}
		}

		// Emissive
		str = "";
		aiMat->GetTexture(aiTextureType_EMISSIVE, 0, &str);
		newMat.emissiveTexureName = std::string(str.C_Str());
		texturePath = m_filePath;
		texturePath.replace_filename(newMat.emissiveTexureName);
		// テクスチャ読み込み
		if (!newMat.emissiveTexureName.empty())
		{
			newMat.spEmissiveTexure = std::make_shared<D3D11_Texture>(spRenderer->GetRHIDevice());
			if (!newMat.spEmissiveTexure)
			{
				return false;
			}
			if (!newMat.spEmissiveTexure->Create(texturePath.string()))
			{
				newMat.spEmissiveTexure = spRenderer->GetWhiteTexture();

			}
		}

		m_materials.push_back(newMat);
	}

	return true;
}

// 行列のオーダー入れ替え
void SwapMatrix4x4Order(const Matrix& _mSrc,
						Matrix&	   _mDest)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			_mDest.m[i][j] = _mSrc.m[j][i];
		}
	}
}

void Model::ProcessNode(aiNode* _pNode, const aiScene* _pScene)
{
	// if (_pNode->mNumMeshes > 0)
	{

		Node newNode;
		// 名前
		newNode.Name = std::string(_pNode->mName.C_Str());

		// ノード番号
		newNode.m_nodeIndex = m_aiNodeIndexMap[_pNode];
		// 親
		if (m_aiNodeIndexMap.count(_pNode->mParent))
		{
			newNode.m_parent = m_aiNodeIndexMap[_pNode->mParent];
		}
		// 子
		const uint32_t numChildren = _pNode->mNumChildren;
		newNode.m_children.reserve(numChildren);
		for (uint32_t i = 0; i < numChildren; ++i)
		{
			newNode.m_children.push_back(
				m_aiNodeIndexMap[_pNode->mChildren[i]]);
		}

		// ローカル、ワールド行列
		// assimpの行列は列オーダーなので、これを入れ替えて行オーダーに変換してあげる必要がある。
		{
			// 列オーダー → 行オーダー
			Matrix mOriginalTrans =
				Matrix(&_pNode->mTransformation.a1);
			Matrix mLocalTrans;
			SwapMatrix4x4Order(mOriginalTrans, mLocalTrans);

			// ローカル行列
			newNode.m_localTransform = mLocalTrans;
			// ワールド行列
			newNode.m_worldTransform =
				(_pScene->mRootNode == _pNode || newNode.m_parent < 0)
					? mLocalTrans
					: mLocalTrans * m_nodes[newNode.m_parent].m_worldTransform;
		}

		// メッシュであるか？
		newNode.IsMesh = (_pNode->mNumMeshes > 0);

		// newNode.m_inverseBindMatrix = node.m_inverseBindMatrix;

		if (_pNode->mNumMeshes > 0)
		{
			// Subset(aiMesh)ごとの情報を取得
			for (unsigned int i = 0; i < _pNode->mNumMeshes; ++i)
			{
				auto mSrc = _pScene->mMeshes[_pNode->mMeshes[i]];

				// 頂点情報
				for (unsigned int j = 0; j < mSrc->mNumVertices; ++j)
				{
					auto		vSrc = mSrc->mVertices[j];
					ModelVertex vDest = {};
					vDest.Pos =
						Vector3(vSrc.x, vSrc.y, vSrc.z);
					// vDest.UV = DirectX::SimpleMath::Vector2(&vSrc.mTex);
					//  UV
					if (mSrc->mTextureCoords[0])
					{
						vDest.UV.x = mSrc->mTextureCoords[0][j].x;
						vDest.UV.y = mSrc->mTextureCoords[0][j].y;
						// DirectXはUV座標系が左上原点なので、V値を反転させる
						vDest.UV.y = 1.0f - vDest.UV.y;
					}

					// 頂点カラー
					/*auto aiMat = _pScene->mMaterials[mSrc->mMaterialIndex];

					aiColor3D color;
					aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, color);

					float uintVias = static_cast<float>(0xF);
					uint32_t uiCol[3] = {
						static_cast<uint32_t>(color.r * uintVias),
						static_cast<uint32_t>(color.g * uintVias),
						static_cast<uint32_t>(color.b * uintVias)
					};
					vDest.Color = uiCol[0] << 6 | uiCol[1] << 3 | uiCol[2];*/

					newNode.Verteces.push_back(vDest);
				}

				// 面情報
				const int faceStartIdx = (int)newNode.Faces.size();
				for (uint32_t j = 0; j < mSrc->mNumFaces; j++)
				{
					aiFace fSrc = mSrc->mFaces[j];

					ModelFace fDest = {};
					fDest.idx[0] = fSrc.mIndices[0];
					fDest.idx[1] = fSrc.mIndices[1];
					fDest.idx[2] = fSrc.mIndices[2];
					newNode.Faces.push_back(fDest);
				}

				// Material
				if (mSrc->mMaterialIndex >= 0)
				{
					Subset sDest = {};
					sDest.materialIdx = mSrc->mMaterialIndex;
					sDest.faceStartIdx = faceStartIdx;
					sDest.faceCount = mSrc->mNumFaces;
					newNode.Subsets.push_back(sDest);
				}
			}
		}

		m_nodes.push_back(newNode);
	}

	for (uint32_t i = 0; i < _pNode->mNumChildren; i++)
	{
		this->ProcessNode(_pNode->mChildren[i], _pScene);
	}
}

void ModelWork::Set(std::shared_ptr<Model>& _pModel)
{
	if (!_pModel)
	{
		return;
	}
	m_spModelData = _pModel;

	const uint32_t nodeSize = (uint32_t)m_spModelData->GetNodes().size();
	m_coppiedNodes.resize(nodeSize);

	auto& orgNodes = _pModel->GetNodes();

	// ノードをコピーしてくる
	for (uint32_t i = 0; i < nodeSize; ++i)
	{
		m_coppiedNodes[i] = orgNodes[i];
	}
}

void ModelWork::CalcWorldMatrixes()
{
	if (!m_spModelData)
	{
		assert(0 && "モデルのないノード行列計算");
		return;
	}

	// 全ボーン行列を書き込み
	// std::map<const void*, int>& rNodeIndexMap = m_aiNodeIndexMap;
	std::function<void(int, int)> rec =
		[&rec, this](int nodeIdx, int parentNodeIdx)
	{
		auto& data = this->m_spModelData->GetNodes()[nodeIdx];
		auto& work = this->m_coppiedNodes[nodeIdx];

		// 親との行列を合成
		if (parentNodeIdx >= 0)
		{
			auto& parent = m_coppiedNodes[data.m_parent];

			work.m_worldTransform =
				work.m_localTransform * parent.m_worldTransform;
		}
		// 親が居ない場合は親は自分自身とする
		else
		{
			work.m_worldTransform = work.m_localTransform;
		}

		for (auto childNodeIdx : data.m_children)
		{
			rec(childNodeIdx, nodeIdx);
		}
	};

	if (m_spModelData->GetRootNodeIndices().size() > 0)
	{
		for (int rootIdx : m_spModelData->GetRootNodeIndices())
		{
			rec(rootIdx, -1);
		}
	}
	else
	{
		rec(0, -1);
	}

	/*for (auto&& nodeIdx : m_spModelData->GetNodes()[0].m_children)
	{
		rec(nodeIdx, -1);
	}*/
	/*for (uint32_t nodeIdx = 0; nodeIdx < m_coppiedNodes.size(); ++nodeIdx)
	{
		m_worldMatrixes[nodeIdx] =
	m_spModelData->GetNodes()[nodeIdx].m_localTransform * m_mOrigin;
	}*/
}


//=================================================
// Animation(WIP)
//=================================================


//void ModelWork::Animation(float speed)
//{
//	static float now = 0.0f;
//	auto&&		 animData = m_spModelData->GetAnimations()[1];
//
//	now += speed;
//	if (animData.Length < now)
//	{
//		now = 0;
//	}
//
//	for (auto&& animNode : animData.Nodes)
//	{
//		auto& nodeMat =
//			m_coppiedNodes[animNode.ModelNodeIndex].m_localTransform;
//		auto& localMat =
//			m_spModelData->GetNodes()[animNode.ModelNodeIndex].m_localTransform;
//
//		/*uint32_t useIdx = (uint32_t)now;
//		if (useIdx >= animNode.Rotations.size())
//		{
//			useIdx = animNode.Rotations.size() - 1;
//		}
//		DirectX::SimpleMath::Matrix rot =
//			DirectX::SimpleMath::Matrix::CreateFromQuaternion(
//				animNode.Rotations[useIdx].Quat);*/
//
//		Matrix rot;
//		if (animNode.Rotations.size() > 1)
//		{
//			uint32_t useIdx = (uint32_t)now;
//			for (useIdx = 0; useIdx < animNode.Rotations.size() - 1; useIdx++)
//			{
//				if (now < animNode.Rotations[useIdx + 1].Time)
//				{
//					break;
//				}
//			}
//			uint32_t lastKey = animNode.Rotations.size() - 2;
//			if (useIdx > lastKey)
//			{
//				useIdx = lastKey;
//			}
//
//			auto& nowRKey = animNode.Rotations[useIdx];
//			auto& nextRKey = animNode.Rotations[useIdx + 1];
//			nowRKey.Interpolation(rot, nextRKey, now + 1);
//		}
//		// 補完する必要がない場合
//		else if (animNode.Rotations.size() == 1)
//		{
//			rot = Matrix::CreateFromQuaternion(
//				animNode.Rotations[0].Quat);
//		}
//
//		nodeMat = rot * localMat;
//	}
//}
//
//bool ModelAnimation::Node::RotateKey::Interpolation(
//	Matrix& mOut, const RotateKey& next,
//	float progressTime) const
//{
//	if (progressTime < Time)
//	{
//		mOut = Matrix::CreateFromQuaternion(Quat);
//		return false;
//	}
//	if (Time > next.Time)
//	{
//		mOut = Matrix::CreateFromQuaternion(next.Quat);
//		return false;
//	}
//
//	float betweenTime = next.Time - Time;
//	float nowPer = (progressTime - Time) / betweenTime;
//
//	auto lQuat =
//		Quaternion::Slerp(Quat, next.Quat, nowPer);
//	mOut = Matrix::CreateFromQuaternion(lQuat);
//
//	return true;
//}
//
//bool ModelAnimation::Node::TransKey::Interpolation(
//	Matrix& mOut, const TransKey& next,
//	float progressTime) const
//{
//	if (progressTime < Time)
//	{
//		mOut = Matrix::CreateTranslation(vPos);
//		return false;
//	}
//	if (Time > next.Time)
//	{
//		mOut = Matrix::CreateTranslation(next.vPos);
//		return false;
//	}
//
//	float betweenTime = next.Time - Time;
//	float nowPer = (progressTime - Time) / betweenTime;
//
//	auto result = DirectX::XMVectorLerp(vPos, next.vPos, nowPer);
//	mOut = DirectX::SimpleMath::Matrix::CreateTranslation(result);
//
//	return true;
//}

}