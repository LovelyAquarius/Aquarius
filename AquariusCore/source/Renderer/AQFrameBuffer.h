#pragma once
#include "core/AquariusCore.h"
#include "core/AQObject.h"

namespace Aquarius
{
	struct AQFrameBufferConfiguration
	{
		AQUINT Width, Height;
		AQUINT Samples = 1;
		AQBOOL SWapChainTarget = false;
	};

	class AQUARIUS_API AQFrameBuffer:public AQObject
	{
	public:
		static AQRef<AQFrameBuffer> Create(const std::string name , AQFrameBufferConfiguration& config);
		static AQRef<AQFrameBuffer> Create(AQFrameBufferConfiguration& config);
	public:
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		virtual void Delete()const = 0;

		virtual AQUINT GetID() = 0;
		virtual const AQFrameBufferConfiguration& GetConfiguration() const = 0;

		virtual void Resize(AQUINT width, AQUINT height) = 0;
	protected:
		AQFrameBuffer(){ m_type = AQObjectType::AQFrameBuffer; }
	
	};









}


