#pragma once
#include <GLAD/glad.h>
#include "core/Log.h"

namespace Aquarius
{

#ifdef AQ_DEBUG

	#define GLASSERT(x)if(!(x)) __debugbreak()

	#define GLCALL(x) GLClearError();\
			x;\
			GLASSERT(GLLog(#x,__FILE__,__LINE__))
#else

	#define GLASSERT(x)
	#define GLCALL(x) x

#endif


	extern void  AQUARIUS_API GLClearError();
	extern bool  AQUARIUS_API GLLog(const char* function, const char* file, int line);
	
}







