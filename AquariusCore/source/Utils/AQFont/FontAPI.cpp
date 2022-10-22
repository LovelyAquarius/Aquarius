#include "AQPCH.h"
#include "Utils/AQFont/STBTureTypeAPI.h"
#include "core/AQCommon.h"


namespace Aquarius
{
	FontAPI AQFontAPI::s_FontAPI = FontAPI::stb_truetype;
	AQFontAPI* AQFontAPI::s_AQFontAPI = nullptr;


	void AQFontAPI::CreateAPI()
	{
		switch (AQFontAPI::GetFontAPI())
		{
			case FontAPI::None:
			{AQ_CORE_ASSERT(false, "FontAPI::NO FontAPI is supported！"); break; }
			case FontAPI::stb_truetype:
			{s_AQFontAPI = new STBTureTypeAPI();  break; }
		}

	}


}