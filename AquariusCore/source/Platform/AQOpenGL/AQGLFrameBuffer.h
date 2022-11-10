#pragma once
#include "core/AquariusCore.h"
#include "Renderer/AQFrameBuffer.h"
#include <GLAD/glad.h>

namespace Aquarius
{
	class AQUARIUS_API AQGLFrameBuffer:public AQFrameBuffer
	{
	public:
		static AQRef<AQFrameBuffer> Create(const std::string& name, AQFrameBufferConfiguration& config);
		static AQRef<AQFrameBuffer> Create(AQFrameBufferConfiguration& config);

	public:

		AQGLFrameBuffer(AQGLFrameBuffer&) = delete;
		virtual ~AQGLFrameBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual void Delete()const override;

		virtual AQUINT GetID() override { return m_ColorAttachment;}
		virtual const AQFrameBufferConfiguration& GetConfiguration()const override { return m_Configuration; }
		
		virtual void Resize(AQUINT width, AQUINT height)override;
		
		
		GLuint GetFBO() { return m_FBO; }
		void Invalidate();



	protected:
		AQGLFrameBuffer(const AQFrameBufferConfiguration& config);
		AQGLFrameBuffer(const std::string& name, const AQFrameBufferConfiguration& config);
	private:
		AQFrameBufferConfiguration m_Configuration;
		GLuint m_FBO;
		GLuint m_ColorAttachment;
		GLuint m_DepthAttachment;
	};

}