#include "AQPCH.h"
#include "GLError.h"



namespace Aquarius
{

	void GLClearError()
	{
		while (glGetError() != GL_NO_ERROR);
	}

	bool GLLog(const char* function, const char* file, int line)
	{
		while (GLenum error = glGetError())
		{
			AQ_CORE_ERROR("[OPenGL Error]({0}){1}:{2},{3}!", error, function, file, line);
			return false;
		}

		return true;

	}




}