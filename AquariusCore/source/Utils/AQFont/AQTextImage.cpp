#include "AQPCH.h"
#include "AQTextImage.h"
#include "core/AQCommon.h"


namespace Aquarius
{
	std::string  AQTextImage::fontfilepath = R"(G:\Mine\CppProjects\Aquarius\AquariusCore\source\Data\Fonts\msyh.ttc)";
	std::string  AQTextImage::fontname = AQ_ExtractFilename(R"(G:\Mine\CppProjects\Aquarius\AquariusCore\source\Data\Fonts\simhei.ttf)");

	//void AQTextImage::Init()
	//{
	//	//初始化字体API
	//	switch (AQFontAPI::GetFontAPI())
	//	{
	//		case FontAPI::None:
	//		{AQ_CORE_ASSERT(false, "FontAPI::NO FontAPI is supported！"); break; }
	//		case FontAPI::stb_truetype:
	//		{AQTextImage::s_AQFontAPI = new STBTureTypeAPI();  break; }
	//	}
	//	//________________________________________
	//	//初始化字体信息
	//	AQTextImage::fontfilepath = R"(.\source\Data\Fonts\msyh.ttc)";
	//	AQTextImage::fontname = AQ_ExtractFilename(R"(.\source\Data\Fonts\msyh.ttc)");
	//	AQTextImage::s_AQFontAPI->LoadFont(fontfilepath);
	//}

}
