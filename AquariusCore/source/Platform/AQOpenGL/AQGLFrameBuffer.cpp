#include "AQPCH.h"
#include "AQGLFrameBuffer.h"
#include "core/Application.h"
#include "MathSystem/AQMathEx.h"
#include "GLError.h"
#include<GLFW/glfw3.h>

namespace Aquarius
{
	static AQUINT s_MaxFrameBuffersize = 8192;
	extern Eigen::Vector2ui GlobalViewPortSize;
	static AQBOOL IsDepthFormat(FrameBufferTexutureFormat format)
	{
		switch (format)
		{
			case(FrameBufferTexutureFormat::DEPTH24STENCIL8):return true;
		}
			
		return false;
	}
	static GLenum TextureTarget(bool multisampled)
	{
		return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}
	static void CreateTextures(bool multisampled, GLuint* outid, AQUINT count)
	{
		GLCALL(glCreateTextures(TextureTarget(multisampled), count, outid));
	}
	static void BindTexture(bool multisampled, GLuint id)
	{
		GLCALL(glBindTexture(TextureTarget(multisampled), id));
	}
	static void AttachColorTexture(GLuint id, AQUINT samples, GLenum internalformat, GLenum format, AQUINT width, AQUINT height, AQINT index)
	{
		bool ismultisampled = samples > 1;
		if (ismultisampled)
		{
			GLCALL(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalformat, width, height, GL_FALSE));
		}
		else
		{
			GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr));
			GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
			GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		}
		GLCALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+index, TextureTarget(ismultisampled), id, 0));
	}
	static void AttachDepthTexture(GLuint id, AQUINT samples, GLenum format, GLenum type, AQUINT width, AQUINT height)
	{
		bool ismultisampled = samples > 1;
		if (ismultisampled)
		{
			GLCALL(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE));
		}
		else
		{
			GLCALL(glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height));
			GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
			GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		}
		GLCALL(glFramebufferTexture2D(GL_FRAMEBUFFER, type, TextureTarget(ismultisampled), id, 0));
	}
	static GLenum ParseFormat(FrameBufferTexutureFormat format)
	{
		switch (format)
		{
		case(FrameBufferTexutureFormat::RGBA8): return GL_RGBA8;
		case(FrameBufferTexutureFormat::REDI): return GL_RED_INTEGER;
		case(FrameBufferTexutureFormat::DEPTH24STENCIL8): return GL_DEPTH24_STENCIL8;
		}
		AQ_ASSERT(false,"ParseFormat:Unknow Format!")
		return 0;
	}
}
namespace Aquarius
{
	AQRef<AQFrameBuffer> AQGLFrameBuffer::Create(const std::string& name, AQFrameBufferConfiguration& config)
	{
		return new AQGLFrameBuffer(name, config);
	}

	AQRef<AQFrameBuffer> AQGLFrameBuffer::Create(AQFrameBufferConfiguration& config)
	{
		return new AQGLFrameBuffer(config);
	}

	void AQGLFrameBuffer::Updateformat(const AQFrameBufferConfiguration& config)
	{
		for (auto config : config.Attachment.Attachments)
		{
			if (IsDepthFormat(config.TextureFormat))
				m_DepthAttachmentConfig = config;
			else
				m_ColorAttachmentsConfig.emplace_back(config);
		}
	}


	AQGLFrameBuffer::AQGLFrameBuffer(const AQFrameBufferConfiguration& config)
		:m_Configuration(config),m_FBO(0)
	{
		AQ_INITIALIZE_AQOBJECT_TYPE(AQGLFrameBuffer);
		Updateformat(m_Configuration);
		Invalidate();
	}

	AQGLFrameBuffer::AQGLFrameBuffer(const std::string& name, const AQFrameBufferConfiguration& config)
		: m_Configuration(config), m_FBO(0)
	{
		AQ_INITIALIZE_AQOBJECT_NAME_AND_TYPE(name, AQGLFrameBuffer);
		Updateformat(m_Configuration);
		Invalidate();
	}



	AQGLFrameBuffer::~AQGLFrameBuffer()
	{
		if(m_FBO)
			GLCALL(glDeleteFramebuffers(1, &m_FBO));
		GLCALL(glDeleteTextures((GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data()));
		GLCALL(glDeleteTextures(1, &m_DepthAttachment));
	}
	void AQGLFrameBuffer::Bind() const
	{
		GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));
		GLCALL(glViewport(0, 0, m_Configuration.Width, m_Configuration.Height));
		GlobalViewPortSize << m_Configuration.Width, m_Configuration.Height;
	}
	void AQGLFrameBuffer::UnBind() const
	{
		GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}
	void AQGLFrameBuffer::Delete() 
	{
		GLCALL(glDeleteFramebuffers(1, &m_FBO));
		GLCALL(glDeleteTextures((GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data()));
		GLCALL(glDeleteTextures(1, &m_DepthAttachment));
		m_ColorAttachments.clear();
		m_DepthAttachment = 0;
	}
	AQINT AQGLFrameBuffer::ReadPixel(AQUINT attachmentindex, AQINT x, AQINT y)
	{
		AQ_ASSERT(attachmentindex < m_ColorAttachments.size(), "AQGLFrameBuffer::ReadPixel: index too large!");
		//GLCALL(glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentindex));
		GLCALL(glNamedFramebufferReadBuffer(m_FBO, GL_COLOR_ATTACHMENT0 + attachmentindex));
		int pixeldata;
		GLCALL(glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixeldata));
		return pixeldata;
	}
	Eigen::Vector4f AQGLFrameBuffer::ReadPixelRGBA(AQUINT attachmentindex, AQINT x, AQINT y)
	{
		AQ_ASSERT(attachmentindex < m_ColorAttachments.size(), "AQGLFrameBuffer::ReadPixel: index too large!");
		GLCALL(glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentindex));
		float rgba[4]={0.0f,0.0f,0.0f,0.0f};
		Eigen::Vector4f pixeldata;
		GLCALL(glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, pixeldata.data()));
		return pixeldata;
	}

	void AQGLFrameBuffer::ClearAttachment(AQUINT attachmentindex, int value)
	{
		AQ_ASSERT(attachmentindex < m_ColorAttachments.size(), "AQGLFrameBuffer::ClearAttachment: index too large!");
		auto& config = m_ColorAttachmentsConfig[attachmentindex];
		GLCALL(glClearTexImage(m_ColorAttachments[attachmentindex], 0, ParseFormat(config.TextureFormat), GL_INT, &value));

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
			GLCALL(glDeleteTextures((GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data()));
			GLCALL(glDeleteTextures(1, &m_DepthAttachment));
			m_ColorAttachments.clear();
			m_DepthAttachment = 0;
		}
		GLCALL(glCreateFramebuffers(1, &m_FBO));
		GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));

		bool ismultisampled = m_Configuration.Samples > 1;

		if (m_ColorAttachmentsConfig.size())
		{
			m_ColorAttachments.resize(m_ColorAttachmentsConfig.size());
			CreateTextures(ismultisampled, m_ColorAttachments.data(), (AQUINT)m_ColorAttachments.size());

			for (size_t i = 0; i < m_ColorAttachmentsConfig.size(); i++)
			{
				BindTexture(ismultisampled, m_ColorAttachments[i]);
				switch (m_ColorAttachmentsConfig[i].TextureFormat)
				{
					case(FrameBufferTexutureFormat::RGBA8):
					{
						AttachColorTexture(m_ColorAttachments[i], m_Configuration.Samples, GL_RGBA8, GL_RGBA, m_Configuration.Width, m_Configuration.Height, (int)i);
						break;
					}
					case(FrameBufferTexutureFormat::REDI):
					{
						AttachColorTexture(m_ColorAttachments[(int)i], m_Configuration.Samples, GL_R32I, GL_RED_INTEGER, m_Configuration.Width, m_Configuration.Height, (int)i);
						break;
					}
				}
			}
		}

		if (m_DepthAttachmentConfig.TextureFormat != FrameBufferTexutureFormat::None)
		{
			CreateTextures(ismultisampled, &m_DepthAttachment, 1);
			BindTexture(ismultisampled, m_DepthAttachment);
			switch (m_DepthAttachmentConfig.TextureFormat)
			{
				case(FrameBufferTexutureFormat::DEPTH24STENCIL8):
				{
					AttachDepthTexture(m_DepthAttachment, m_Configuration.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Configuration.Width, m_Configuration.Height);
					break;
				}
			}
		}

		if (m_ColorAttachments.size())
		{
			AQ_ASSERT(m_ColorAttachments.size() <= 4, "FrameBuffer ColorAttachments>4 !");
			const GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 ,GL_COLOR_ATTACHMENT2 ,GL_COLOR_ATTACHMENT3 };
			GLCALL(glDrawBuffers((GLsizei)m_ColorAttachments.size(), buffers));
		}
		else if (m_ColorAttachments.empty())
		{
			GLCALL(glDrawBuffer(GL_NONE));
		}


		GLCALL(AQ_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "AQGLFrameBuffer::Invalidate:Failed to Generate FrameBuffer!"));
		GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}



}