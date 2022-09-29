#include "AQPCH.h"
#include "OpenGLRenderAPI.h"

#include "GLError.h"

namespace Aquarius
{


	void OpenGLRenderAPI::SetClearcolor(const glm::vec4& color)
	{
		GLCALL(glClearColor(color.r, color.g, color.b, color.a));
	}

	void OpenGLRenderAPI::Clear()
	{
		GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void OpenGLRenderAPI::DrawElement(const std::shared_ptr<AQVertexArray>& VAO, uint32_t elementlocation)
	{
		GLCALL(glDrawArrays(GL_TRIANGLES, 0, VAO->GetLayout().GetElements()[elementlocation].Count));
		/*GLCALL(glDrawElements(GL_TRIANGLES, VAO->GetLayout().GetElements()[elementlocation].Count, GL_UNSIGNED_INT, nullptr));*/
	}


}