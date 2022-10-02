#pragma once
#include "core/AquariusCore.h"

namespace Aquarius
{
	class AQUARIUS_API AQTexture
	{
	public:
		virtual const int GetWidth()const = 0;
		virtual const int GetHeight()const = 0;


	};

	class AQUARIUS_API AQTexture2D:public AQTexture 
	{
	public:
		static AQTexture2D* Create(const char* filename);
		static AQTexture2D* Create();
	
	
	public:
		virtual const int GetWidth()const = 0;
		virtual const int GetHeight()const = 0;
	};




}


