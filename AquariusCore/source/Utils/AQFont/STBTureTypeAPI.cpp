#include "AQPCH.h"
#include "STBTureTypeAPI.h"
#include "core/AQCommon.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include"stb_image_write/stb_image_write.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype/stb_truetype.h>


namespace Aquarius
{
	unsigned char* STBTureTypeAPI::fontdata = nullptr;
	stbtt_fontinfo STBTureTypeAPI::fontinfo= stbtt_fontinfo();


	void STBTureTypeAPI::Init()
	{

	}

	void STBTureTypeAPI::Clear()
	{
	}

	void STBTureTypeAPI::LoadFont(const std::string& filepath)
	{
		fontdata = AQ_LoadFile_U8(filepath);

		std::string filename = AQ_ExtractFilename(filepath);
		int offset = 0;
		if (filename.find("ttc"))
			offset = stbtt_GetFontOffsetForIndex(fontdata, 0);//读取ttc文件时暂时只读取第一套字体！

		if (!stbtt_InitFont(&fontinfo, fontdata, offset))
		{
			AQ_CORE_ERROR("STBTureTypeAPI::LoadFont :Failed to load font from filepath: {0}", filepath);
			return;
		}
	}

	AQFontAPI::bitmap* STBTureTypeAPI::GetSinglecharacterBitmap(const unsigned int unicode,const int pixelsize)
	{
		int bound_x1, bound_x2, bound_y1, bound_y2;
		int width, height;
		float scale = stbtt_ScaleForPixelHeight(&fontinfo, pixelsize);

		stbtt_GetCodepointBitmapBox(&fontinfo, unicode, scale, scale, &bound_x1, &bound_y1, &bound_x2, &bound_y2);
		width = bound_x2 - bound_x1;
		height = bound_y2 - bound_y1;
		AQFontAPI::bitmap* bitmap = new AQFontAPI::bitmap(width, height, 1);
		stbtt_MakeCodepointBitmap(&fontinfo, (unsigned char*)bitmap->data, width, height, width, scale, scale, unicode);
		
		return bitmap;
	}

	void STBTureTypeAPI::GetSinglecharacterBezier(const unsigned int unicode, std::vector<AQRef<AQQuadraticBezierCurve2D>>& beziershape, float scale)
	{
		std::vector<AQRef<AQQuadraticBezierCurve2D>>().swap(beziershape);
		stbtt_vertex* vertices;
		int vertexcount = stbtt_GetCodepointShape(&fontinfo, unicode, &vertices);
		AQRef<AQQuadraticBezierCurve2D> temp;

		for (int index = 0; index < vertexcount; index++)
		{
			stbtt_vertex* vertex = vertices + index;
			
			switch (vertex->type)
			{
				case(STBTT_vmove):
				{
					if (beziershape.size())
					{
						temp->AddPoint(temp->GetFirstPoint(), AQ2DCoord((temp->GetLastPoint().x + temp->GetFirstPoint().x) / 2, (temp->GetLastPoint().y + temp->GetFirstPoint().y) / 2));
					}
					temp = AQQuadraticBezierCurve2D::Create();
					beziershape.emplace_back(temp);
					temp->SetStartpoint(AQ2DCoord(vertex->x*scale, vertex->y * scale));
					break;
				}
				case(STBTT_vline):
				{
					temp->AddPoint(AQ2DCoord(vertex->x * scale, vertex->y * scale), AQ2DCoord((temp->GetLastPoint().x + vertex->x * scale) / 2, (temp->GetLastPoint().y + vertex->y * scale) / 2));
					break;
				}
				case(STBTT_vcurve):
				{
					temp->AddPoint(AQ2DCoord(vertex->x * scale, vertex->y * scale), AQ2DCoord(vertex->cx * scale, vertex->cy * scale));
					break;
				}
			}
		}
		beziershape.back()->AddPoint(beziershape.back()->GetFirstPoint(), AQ2DCoord((beziershape.back()->GetLastPoint().x + beziershape.back()->GetFirstPoint().x) / 2, (beziershape.back()->GetLastPoint().y + beziershape.back()->GetFirstPoint().y) / 2));//暂时不知为什么缺最后一个点

		//整个形状全部居中
		AQ2DCoord  center={0.0f,0.0f};
		for (int i = 0; i < beziershape.size(); i++)
		{
			center += beziershape[i]->GetCenter();
		}
		center = center / beziershape.size();

		for (int i = 0; i < beziershape.size(); i++)
		{
			beziershape[i]->MoveCurve(-center);
		}
	}

	void STBTureTypeAPI::WriteBitmapToPNG(const std::string& filename, bitmap* bitmap)
	{
		stbi_write_png(filename.c_str(), bitmap->pixelwidth, bitmap->pixelheight, bitmap->channel, bitmap->data, bitmap->pixelwidth);
	}

}
