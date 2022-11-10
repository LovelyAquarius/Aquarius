#include "AQPCH.h"
#include "AQFrameBuffer.h"
#include "Platform/AQOpenGL/AQGLFrameBuffer.h"
#include "Renderer.h"

namespace Aquarius
{

	AQRef<AQFrameBuffer> AQFrameBuffer::Create(const std::string name, AQFrameBufferConfiguration& config)
	{
		switch (Renderer::GetGraphicAPI())
		{
		case GraphicAPI::None:
		{AQ_CORE_ASSERT(false, "RenderAPI::NO GraphicAPI is supported！"); break; }
		case GraphicAPI::OpenGL:
		{return  AQGLFrameBuffer::Create(name, config); }
		}
		AQ_CORE_ASSERT(false, "RenderAPI::Unknown GraphicAPI!");
		return nullptr;
	}

	AQRef<AQFrameBuffer> AQFrameBuffer::Create(AQFrameBufferConfiguration& config)
	{
		switch (Renderer::GetGraphicAPI())
		{
		case GraphicAPI::None:
		{AQ_CORE_ASSERT(false, "RenderAPI::NO GraphicAPI is supported！"); break; }
		case GraphicAPI::OpenGL:
		{return  AQGLFrameBuffer::Create(config); }
		}
		AQ_CORE_ASSERT(false, "RenderAPI::Unknown GraphicAPI!");
		return nullptr;
	}

}
