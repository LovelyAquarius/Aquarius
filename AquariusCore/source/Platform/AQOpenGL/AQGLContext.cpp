#include "AQPCH.h"

#include "AQGLContext.h"
#include "core/Log.h"

namespace Aquarius
{


	AQGLContext::AQGLContext(GLFWwindow* windowhandle)
		:m_WindowHandle(windowhandle)
	{

		AQ_CORE_ASSERT(windowhandle, "windowhandle is null!");
	
	}

	void AQGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		AQ_CORE_ASSERT(status, "Failed to initialize Glad!");
	}

	void AQGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}


