#pragma once
#include "core/AquariusCore.h"
#include "MathSystem/AQMathEx.h"
#include "core/AQObject.h"

namespace Aquarius
{
	enum class FrameBufferTexutureFormat
	{
		None = 0,
		//颜色格式
		RGBA8, REDI,
		//深度模式
		DEPTH24STENCIL8,
		//默认值
		DEPTH = DEPTH24STENCIL8
	};
	struct FrameBufferTextureConfiguration
	{
		FrameBufferTextureConfiguration() = default;
		FrameBufferTextureConfiguration(FrameBufferTexutureFormat format) :TextureFormat(format) {}
		FrameBufferTexutureFormat TextureFormat = FrameBufferTexutureFormat::None;
	};
	struct FrameBufferAttachmentConfiguration
	{
		FrameBufferAttachmentConfiguration() = default;
		FrameBufferAttachmentConfiguration(std::initializer_list<FrameBufferTextureConfiguration> attatchments):Attachments(attatchments){}
		std::vector<FrameBufferTextureConfiguration> Attachments;

	};



	struct AQFrameBufferConfiguration
	{
		AQUINT Width, Height;
		AQUINT Samples = 1;
		AQBOOL SWapChainTarget = false;
		FrameBufferAttachmentConfiguration Attachment;
	};

	class AQUARIUS_API AQFrameBuffer:public AQObject
	{
	public:
		static AQRef<AQFrameBuffer> Create(const std::string name , AQFrameBufferConfiguration& config);
		static AQRef<AQFrameBuffer> Create(AQFrameBufferConfiguration& config);
	public:
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		virtual void Delete() = 0;
		virtual const std::vector<AQUINT>& GetColorAttachments() const = 0;
		virtual AQUINT GetColorAttachmentID(AQUINT index) const = 0;
		virtual const AQFrameBufferConfiguration& GetConfiguration() const = 0;

		virtual void Resize(AQUINT width, AQUINT height) = 0;
		virtual AQINT ReadPixel(AQUINT attachmentindex, AQINT x, AQINT y) = 0;
		virtual Eigen::Vector4f ReadPixelRGBA(AQUINT attachmentindex, AQINT x, AQINT y) = 0;
		virtual void ClearAttachment(AQUINT attachmentindex, int value) = 0;
	protected:
		AQFrameBuffer(){ m_type = AQObjectType::AQFrameBuffer; }
	
	};









}


