#include "AQPCH.h"
#include "OpenGLRenderAPI.h"
#include "Platform/AQOpenGL/AQGLBuffer.h"

#include "GLError.h"

namespace Aquarius
{
	Eigen::Vector2ui GlobalViewPortSize;
	void OpenGLRenderAPI::Init()
	{
		
		GLCALL(glEnable(GL_BLEND));
		GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCALL(glEnable(GL_DEPTH_TEST));
	}
	void OpenGLRenderAPI::SetViewport(AQUINT x, AQUINT y, AQUINT width, AQUINT height)
	{
		GLCALL(glViewport(x, y, width, height));
		GlobalViewPortSize << width, height;
	}
	Eigen::Vector2ui OpenGLRenderAPI::GetViewport()
	{
		return GlobalViewPortSize;
	}
	void OpenGLRenderAPI::SetClearcolor(const Eigen::Vector4f& color)
	{
		GLCALL(glClearColor(color.x(), color.y(), color.z(), color.w()));
	}

	void OpenGLRenderAPI::Clear()
	{
		GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void OpenGLRenderAPI::SetBlend()
	{
		/*GLCALL(glBlendFunc(GL_SRC1_ALPHA, GL_ONE_MINUS_SRC_ALPHA));*/
	}

	void OpenGLRenderAPI::DrawPointElement(const AQRef<AQVertexArray>& VAO, AQFLOAT pointsize, AQUINT elementcount)
	{
		GLCALL(glDisable(GL_PROGRAM_POINT_SIZE));
		GLCALL(glPointSize(pointsize));
		GLCALL(glDrawElements(GL_POINTS, elementcount, GL_UNSIGNED_INT, nullptr));
	}
	 
	void OpenGLRenderAPI::DrawPointElement(const AQRef<AQVertexArray>& VAO, AQUINT elementcount)
	{
		GLCALL(glEnable(GL_PROGRAM_POINT_SIZE));
		GLCALL(glDrawElements(GL_POINTS, elementcount, GL_UNSIGNED_INT, nullptr));
	}

	void OpenGLRenderAPI::DrawTriangleElement( const  AQRef<AQVertexArray>& VAO, AQUINT elementcount)
	{
	    GLCALL(glDrawElements(GL_TRIANGLES, elementcount, GL_UNSIGNED_INT, nullptr));
		GLCALL(glBindTexture(GL_TEXTURE_2D,0));
	}

	void OpenGLRenderAPI::DrawLineElement(const AQRef<AQVertexArray>& VAO, AQUINT elementcount, AQFLOAT width)
	{
		GLCALL(glEnable(GL_LINE_SMOOTH));
		GLCALL(glLineWidth(width));
		GLCALL(glDrawElements(GL_LINE_STRIP, elementcount, GL_UNSIGNED_INT, nullptr));
	}

	void OpenGLRenderAPI::DrawLineElement(const AQRef<AQVertexArray>& VAO, const std::vector<AQUINT>& elementcounts, AQFLOAT width)
	{
		GLCALL(glEnable(GL_LINE_SMOOTH));
		GLCALL(glLineWidth(width));
		int offset=0;
		for (int index = 0; index < elementcounts.size(); index++)
		{
			GLCALL(glDrawElements(GL_LINE_STRIP, elementcounts[index], GL_UNSIGNED_INT, (void*)(offset * sizeof(AQUINT))));
			offset += elementcounts[index];
		}
	}

	void OpenGLRenderAPI::DrawBuffer(const AQRef<AQFrameBuffer>& FBO)
	{

		if (FBO->GetColorAttachments().size())
		{
			AQ_ASSERT(FBO->GetColorAttachments().size() <= 4, "FrameBuffer ColorAttachments>4 !");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1 ,GL_COLOR_ATTACHMENT2 ,GL_COLOR_ATTACHMENT3 };
			GLCALL(glDrawBuffers((GLsizei)FBO->GetColorAttachments().size(), buffers));
		}
		else if (FBO->GetColorAttachments().empty())
		{
			GLCALL(glDrawBuffer(GL_NONE));
		}
		GLCALL(AQ_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "OpenGLRenderAPI::DrawBuffer:Invalidate:Failed to Generate FrameBuffer!"));

	}

	void OpenGLRenderAPI::SetLineWidth(const AQFLOAT width)
	{
		GLCALL(glEnable(GL_LINE_SMOOTH));
		GLCALL(glHint(GL_LINE_SMOOTH_HINT, GL_NICEST));
		GLCALL(glLineWidth(width));
	}



}