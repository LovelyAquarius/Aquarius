#pragma once
#include "core/AquariusCore.h"
#include "Utils/AQFont/FontAPI.h"
#include "Utils/AQGeometry/AQEuclidian2D.h"

struct stbtt_fontinfo;

namespace Aquarius
{

	class AQUARIUS_API STBTureTypeAPI : public AQFontAPI
	{
	public:
		virtual void Init()override;
		virtual void Clear()override;
		virtual void LoadFont(const std::string& filepath)override;
		virtual bitmap* GetSinglecharacterBitmap(const unsigned int unicode,const int pixelsize)override;
		virtual void GetSinglecharacterBezier(const unsigned int unicode, std::vector<AQRef<AQQuadraticBezierCurve2D>>& beziershape ,float scale=0.05)override;
		virtual void WriteBitmapToPNG(const std::string& outfilepath, bitmap* bitmap)override;

	private:
		static unsigned char* fontdata;
		static stbtt_fontinfo fontinfo;
	};

}