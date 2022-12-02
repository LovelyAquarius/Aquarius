#pragma once
#include "core/AquariusCore.h"
#include <string>
namespace Aquarius
{
	class AQUARIUS_API FileDialogs
	{
	public:
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};




}