#pragma once
#include "Renderer/AQTexture.h"
#include "Renderer/AQShader.h"
#include "MathSystem/AQMathEx.h"

namespace Aquarius
{
	class CommonTexture2DData :public AQObject
	{
	//最大纹理槽数量，取决于实现
	static const AQUINT MaxTextureSlots = 32;

	public:
		static AQRef<CommonTexture2DData> Create();
	public:
		//纹理槽数量
		std::array<AQRef<AQTexture2D>, MaxTextureSlots> TextureSlots;
		AQUINT TextureSlotIndex = 1;

		//PixelTexture数据
		AQRef<AQTexture2D> DefaultTexture2D;
		AQRef<AQShader> DefaultTextureShader2D;
	public:
		//检查传入texture是否存在于槽内，如果找到，返回对应槽的index，否则返回0；
		AQUINT FindTexture(const AQRef<AQTexture2D>& texture);
		void ResetData();

	protected:
		CommonTexture2DData();
		CommonTexture2DData(const CommonTexture2DData& other)=delete;

	};


}


