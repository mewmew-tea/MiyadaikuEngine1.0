#include "RHI_Texture.h"

//#include "../../External/stb/stb_image.h"

namespace Miyadaiku
{
//bool RHI_Texture::LoadImageData(std::string_view			_filePath,
//								unsigned char** _ppOutData,
//								int* _pOutWidth, int* _pOutHeight,
//								int* _pOutChannelsInFile,
//								int	 _imageDesiredChannels)
//{
//	unsigned char* imageData =
//		stbi_load(_filePath.data(), _pOutWidth, _pOutHeight,
//				  _pOutChannelsInFile, _imageDesiredChannels);
//
//	if (imageData)
//	{
//		return false;
//	}
//
//	_ppOutData = &imageData;
//
//	return true;
//}

//bool RHI_Texture::LoadImageData(std::string_view			_filePath,
//								std::vector<unsigned char>& _outData,
//								int* _pOutWidth, int* _pOutHeight,
//								int* _pOutChannelsInFile,
//								int	 _imageDesiredChannels)
//{
//	unsigned char* imageData =
//		stbi_load(_filePath.data(), _pOutWidth, _pOutHeight,
//				  _pOutChannelsInFile, _imageDesiredChannels);
//
//	if (imageData)
//	{
//		return false;
//	}
//
//	const int outDataLength =
//		(*_pOutWidth) * (*_pOutHeight) * (*_pOutChannelsInFile);
//	_outData = std::vector<unsigned char>(imageData, imageData + outDataLength);
//}
} // namespace Miyadaiku