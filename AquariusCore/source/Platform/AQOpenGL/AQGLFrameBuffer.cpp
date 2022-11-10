#include "AQPCH.h"
#include "AQGLFrameBuffer.h"
#include "GLError.h"
namespace Aquarius
{
	static AQUINT s_MaxFrameBuffersize = 8192;

	AQRef<AQFrameBuffer> AQGLFrameBuffer::Create(const std::string& name, AQFrameBufferConfiguration& config)
	{
		return new AQGLFrameBuffer(name, config);
	}

	AQRef<AQFrameBuffer> AQGLFrameBuffer::Create(AQFrameBufferConfiguration& config)
	{
		return new AQGLFrameBuffer(config);
	}




	AQGLFrameBuffer::AQGLFrameBuffer(const AQFrameBufferConfiguration& config)
		:m_Configuration(config),m_FBO(0),m_ColorAttachment(0),m_DepthAttachment(0)
	{
		m_Name = "Unnamed AQGLFrameBuffer";
		m_type = AQObjectType::AQGLFrameBuffer;

		Invalidate();
	}

	AQGLFrameBuffer::AQGLFrameBuffer(const std::string& name, const AQFrameBufferConfiguration& config)
		: m_Configuration(config), m_FBO(0), m_ColorAttachment(0), m_DepthAttachment(0)
	{
		if (name.size())
			m_Name = name;
		else
			m_Name = "Unnamed AQGLFrameBuffer";
		m_type = AQObjectType::AQGLFrameBuffer;

		Invalidate();
	}



	AQGLFrameBuffer::~AQGLFrameBuffer()
	{
		if(m_FBO)
			GLCALL(glDeleteFramebuffers(1, &m_FBO));
		GLCALL(glDeleteTextures(1, &m_ColorAttachment));
		GLCALL(glDeleteTextures(1, &m_DepthAttachment));
	}
	void AQGLFrameBuffer::Bind() const
	{
		GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));
		GLCALL(glViewport(0, 0, m_Configuration.Width, m_Configuration.Height));
	}
	void AQGLFrameBuffer::UnBind() const
	{
		GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}
	void AQGLFrameBuffer::Delete() const
	{
		GLCALL(glDeleteFramebuffers(1, &m_FBO));
		GLCALL(glDeleteTextures(1, &m_ColorAttachment));
		GLCALL(glDeleteTextures(1, &m_DepthAttachment));
	}
	void AQGLFrameBuffer::Resize(AQUINT width, AQUINT height)
	{
		if (width == 0 || height == 0 || width > s_MaxFrameBuffersize || height > s_MaxFrameBuffersize)
			return;
		m_Configuration.Width = width;
		m_Configuration.Height = height;
		Invalidate();
	}
	void AQGLFrameBuffer::Invalidate()
	{
		if (m_FBO)
		{
			GLCALL(glDeleteFramebuffers(1, &m_FBO));
			GLCALL(glDeleteTextures(1, &m_ColorAttachment));
			GLCALL(glDeleteTextures(1, &m_DepthAttachment));
		}


		GLCALL(glCreateFramebuffers(1, &m_FBO));
		GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));

		GLCALL(glCreateTextures(GL_TEXTURE_2D, 1,&m_ColorAttachment));
		GLCALL(glBindTexture(GL_TEXTURE_2D, m_ColorAttachment));
		GLCALL(glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA8, m_Configuration.Width, m_Configuration.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

		GLCALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0));
		GLCALL(glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment));
		GLCALL(glBindTexture(GL_TEXTURE_2D, m_DepthAttachment));
		GLCALL(glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Configuration.Width, m_Configuration.Height));
		//GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Configuration.Width, m_Configuration.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr));
		GLCALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0));

		GLCALL(AQ_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "AQGLFrameBuffer::Invalidate:Failed to Generate FrameBuffer!"));
		GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

}