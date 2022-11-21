#pragma once
#include "../RenderingResource.h"
#include <memory>
#include <vector>
#include <string>
#include <string_view>

namespace Miyadaiku
{
class RHI_Device;
class RHI_Texture
{
public:
	// RHI_Texture() = delete;
	RHI_Texture(const std::shared_ptr<RHI_Device>& _spRhiDevice)
	{
		m_wpRHIDevice = _spRhiDevice;
	}
	~RHI_Texture()
	{
	}

	virtual bool Create(RHIResourceHandle_Texture& _srcResource) = 0;
	virtual bool Create(std::string_view _filePath) = 0;
	virtual bool Create(uint32_t _w, uint32_t _h,
						RHI_FORMAT _format = RHI_FORMAT::R8G8B8A8_UNORM,
						uint32_t	   _arrayCnt = 1,
						const void*	   _pFillData = nullptr,
						const uint32_t				  _sysMemPitch = 4) = 0;
	virtual bool CreateDepthStencil(int _w, int _h) = 0;


	virtual void Release() = 0;

	RHIResourceHandle_Texture const& GetTextureHandle()
	{
		return m_textureHandle;
	}
	RHIResourceHandle_SRV const& GetSRVHandle()
	{
		return m_srvHandle;
	}
	RHIResourceHandle_RT const& GetRTHandle()
	{
		return m_rtHandle;
	}
	RHIResourceHandle_DS const& GetDSHandle()
	{
		return m_dsHandle;
	}

protected:
	///**
	// * 画像ファイルからデータを読み込みます。
	// * 
	// * @param _filePath ファイルパス
	// * @param _ppOutData 出力先のデータ
	// * @param _pOutWidth 得られた画像データの横画素数
	// * @param _pOutHeight 得られた画像データの縦画素数
	// * @param _pOutChannelsInFile 得られた画像データのチャンネル数
	// * @param _imageDesiredChannels 画像から抽出するチャンネル数
	// * @return 読み込みの成否
	// */
	//bool LoadImageData(std::string_view			   _filePath,
	//				   unsigned char** _ppOutData, int* _pOutWidth,
	//				   int* _pOutHeight, int* _pOutChannelsInFile,
	//				   int _imageDesiredChannels = 4);
	//bool LoadImageData(std::string_view			   _filePath,
	//				   std::vector<unsigned char>& _outData, int* _pOutWidth,
	//				   int* _pOutHeight, int* _pOutChannelsInFile,
	//				   int _imageDesiredChannels = 4);


	// テクスチャのリソースやビューなど
	RHIResourceHandle_Texture m_textureHandle;
	RHIResourceHandle_SRV	  m_srvHandle;
	RHIResourceHandle_RT	  m_rtHandle;
	RHIResourceHandle_DS	  m_dsHandle;

	std::weak_ptr<RHI_Device> m_wpRHIDevice;
};
} // namespace Miyadaiku