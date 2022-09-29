#pragma once
#include"core/GraphicContext.h"

#include "GLAD/glad.h"
#include "GLFW/glfw3.h"

namespace Aquarius
{




	class AQUARIUS_API AQGLContext : public GraphicContext
	{
	public:
		AQGLContext(GLFWwindow* windowhandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;


	};

}

