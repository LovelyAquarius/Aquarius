#pragma once
#include "core/AquariusCore.h"
#include "core/AQCommon.h"
#include "Utils/AQGeometry/AQEuclidian2D.h"

namespace Aquarius
{
	enum class AQUARIUS_API FontAPI
	{
		None = 0,
		stb_truetype = 1,
		freetype2 = 2
	};

	class AQUARIUS_API AQFontAPI
	{
	public:
		struct bitmap
		{
			int pixelwidth;
			int pixelheight;
			int channel;
			char* data;
			bitmap(const int width,const int height, const int channel):pixelwidth(width), pixelheight(height), channel(channel),data (new char[pixelwidth* pixelheight* channel]) {}
			~bitmap() { delete[] data; }
		};
;

	public:
		static void CreateAPI();
		static AQFontAPI* GetAQFontAPI() { return s_AQFontAPI; }
		static FontAPI& GetFontAPI() { return s_FontAPI; }

	public:
		virtual void Init() = 0;
		virtual void Clear() = 0;
		virtual void LoadFont(const std::string& filepath) = 0;
		virtual bitmap* GetSinglecharacterBitmap(const unsigned int unicode, const int pixelsize)=0;
		virtual void GetSinglecharacterBezier(const unsigned int unicode, std::vector<AQRef<AQQuadraticBezierCurve2D>>& beziershape, float scale = 0.05)=0;//一个字有可能有多条分散曲线，用vector存储n条,玛德有的字体文件给的形状数据特别大，适当用scale来缩放
		virtual void WriteBitmapToPNG(const std::string& outfilepath, bitmap* bitmap) = 0;
	private:
		static FontAPI s_FontAPI;
		static AQFontAPI* s_AQFontAPI;
	};


}
