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

		AQ_CORE_INFO("OpenGL Info:");
		AQ_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		AQ_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		AQ_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

		AQ_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Hazel requires at least OpenGL version 4.5!");
	}

	void AQGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}


