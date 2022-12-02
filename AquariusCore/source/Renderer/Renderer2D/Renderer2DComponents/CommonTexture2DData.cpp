#include "AQPCH.h"
#include "CommonTexture2DData.h"
namespace Aquarius
{
	AQRef<CommonTexture2DData> CommonTexture2DData::Create()
	{
		return new CommonTexture2DData();
	}
	AQUINT CommonTexture2DData::FindTexture(const AQRef<AQTexture2D>& texture)
	{
		for (AQUINT i = 1; i < TextureSlotIndex; i++)
		{
			if (TextureSlots[i] == texture)return i;
		}
		return 0;
	}
	void CommonTexture2DData::ResetData()
	{
		TextureSlotIndex = 1;//第一个默认占用
	}

	CommonTexture2DData::CommonTexture2DData()
	{
		AQ_INITIALIZE_AQOBJECT_TYPE(CommonTexture2DData);

		//配置纹理槽
		int32_t samplers[CommonTexture2DData::MaxTextureSlots];//对每个纹理槽采样
		for (int32_t i = 0; i < CommonTexture2DData::MaxTextureSlots; i++)
			samplers[i] = i;
		//______________________________________________________________
		//设置纯白像素块,把它作成一个单独的纹理是一个高效的方案
		AQUINT WhiteTextureData = 0xffffffff;
		DefaultTexture2D = AQTexture2D::Create("SingleWhitePixel", 1, 1);
		DefaultTexture2D->LoadData(&WhiteTextureData, sizeof(AQUINT));

		const char* whitetextureshader = R"(.\source\Assets\Internal\Shader\texture2d.glsl)";
		DefaultTextureShader2D = AQShader::Create(whitetextureshader);
		DefaultTextureShader2D->Bind();
		DefaultTextureShader2D->SetValue("u_Texture", samplers, CommonTexture2DData::MaxTextureSlots);

		TextureSlots[0] = DefaultTexture2D;
		//________________________________________________________________
	}
}

