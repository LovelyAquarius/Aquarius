#pragma once
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include "AQPCH.h"

bool GLLog_test(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OPenGL Error]" << "," << error << "," << function << "," << file << "," << line << std::endl;
		return false;
	}

	return true;

}

void GLClearError_test()
{
	while (glGetError() != GL_NO_ERROR);
}

#define GLASSERT(x) if(!(x)) __debugbreak()

#define GLCALL(x) GLClearError_test();\
			x;\
			GLASSERT(GLLog_test(#x,__FILE__,__LINE__))

