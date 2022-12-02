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
		virtual void Delete() override;

		virtual const std::vector<AQUINT>& GetColorAttachments() const override { return m_ColorAttachments; }
		virtual AQUINT GetColorAttachmentID(AQUINT index) const override { AQ_ASSERT(index<m_ColorAttachments.size(),"GetColorAttatchmentID:index is too large!"); return m_ColorAttachments[index]; }
		virtual const AQFrameBufferConfiguration& GetConfiguration()const override { return m_Configuration; }
		virtual AQINT ReadPixel(AQUINT attachmentindex, AQINT x, AQINT y)override;
		virtual Eigen::Vector4f ReadPixelRGBA(AQUINT attachmentindex, AQINT x, AQINT y)override;
		virtual void ClearAttachment(AQUINT attachmentindex, int value) override;

		virtual void Resize(AQUINT width, AQUINT height)override;
		
		
		GLuint GetFBO() { return m_FBO; }
		void Invalidate();

	private:
		void Updateformat(const AQFrameBufferConfiguration& config);

	protected:
		AQGLFrameBuffer(const AQFrameBufferConfiguration& config);
		AQGLFrameBuffer(const std::string& name, const AQFrameBufferConfiguration& config);
	private:
		AQFrameBufferConfiguration m_Configuration;
		GLuint m_FBO;

		std::vector<GLuint> m_ColorAttachments;
		std::vector<FrameBufferTextureConfiguration> m_ColorAttachmentsConfig;


		GLuint m_DepthAttachment =0;
		FrameBufferTextureConfiguration m_DepthAttachmentConfig;
	};

}