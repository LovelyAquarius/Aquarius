#include "AQPCH.h"
#include "OpenGLRenderAPI.h"
#include "Platform/AQOpenGL/AQGLBuffer.h"

#include "GLError.h"

namespace Aquarius
{
	void OpenGLRenderAPI::Init()
	{
		GLCALL(glEnable(GL_BLEND));
		GLCALL(glEnable(GL_DEPTH_TEST));
		GLCALL(glBlendFunc(GL_SRC1_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	}
	void OpenGLRenderAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		GLCALL(glViewport(x, y, width, height));
	}
	void OpenGLRenderAPI::SetClearcolor(const glm::vec4& color)
	{
		GLCALL(glClearColor(color.r, color.g, color.b, color.a));
	}

	void OpenGLRenderAPI::Clear()
	{
		GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void OpenGLRenderAPI::DrawPointElement(const AQRef<AQVertexArray>& VAO, float pointsize, uint32_t elementcount)
	{
		GLCALL(glDisable(GL_PROGRAM_POINT_SIZE));
		GLCALL(glPointSize(pointsize));
		uint32_t count = elementcount ? elementcount : AQRefCast<AQGLVertexArray>(VAO)->GetLinkedEBO()->GetElementCount();
		GLCALL(glDrawElements(GL_POINTS, count, GL_UNSIGNED_INT, nullptr));
	}
	 
	void OpenGLRenderAPI::DrawPointElement(const AQRef<AQVertexArray>& VAO, uint32_t elementcount)
	{
		GLCALL(glEnable(GL_PROGRAM_POINT_SIZE));
		uint32_t count = elementcount ? elementcount : AQRefCast<AQGLVertexArray>(VAO)->GetLinkedEBO()->GetElementCount();
		GLCALL(glDrawElements(GL_POINTS, count, GL_UNSIGNED_INT, nullptr));
	}

	void OpenGLRenderAPI::DrawTriangleElement( const  AQRef<AQVertexArray>& VAO, uint32_t elementcount)
	{
		uint32_t count = elementcount ? elementcount:AQRefCast<AQGLVertexArray>(VAO)->GetLinkedEBO()->GetElementCount() ;
	    GLCALL(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
		//GLCALL(glBindTexture(GL_TEXTURE_2D,0));？
	}

	void OpenGLRenderAPI::DrawLineElement(const AQRef<AQVertexArray>& VAO, uint32_t elementcount, float width)
	{
		GLCALL(glEnable(GL_LINE_SMOOTH));
		GLCALL(glLineWidth(width));
		uint32_t count = elementcount ? elementcount : AQRefCast<AQGLVertexArray>(VAO)->GetLinkedEBO()->GetElementCount();
		GLCALL(glDrawElements(GL_LINE_STRIP, elementcount, GL_UNSIGNED_INT, nullptr));
	}

	void OpenGLRenderAPI::DrawLineElement(const AQRef<AQVertexArray>& VAO, const std::vector<uint32_t>& elementcounts, float width)
	{
		GLCALL(glEnable(GL_LINE_SMOOTH));
		GLCALL(glLineWidth(width));
		int offset=0;
		for (int index = 0; index < elementcounts.size(); index++)
		{
			GLCALL(glDrawElements(GL_LINE_STRIP, elementcounts[index], GL_UNSIGNED_INT, (void*)(offset * sizeof(uint32_t))));
			offset += elementcounts[index];
		}
	}

	void OpenGLRenderAPI::SetLineWidth(const float width)
	{
		GLCALL(glEnable(GL_LINE_SMOOTH));
		GLCALL(glHint(GL_LINE_SMOOTH_HINT, GL_NICEST));
		GLCALL(glLineWidth(width));
	}



}