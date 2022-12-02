#include "AQPCH.h"
#include "AQUniformBuffer.h"
#include "Renderer.h"
#include "Platform/AQOpenGL/AQGLUniformBuffer.h"
namespace Aquarius
{
	AQRef<AQUniformBuffer> AQUniformBuffer::Create(AQUINT size, AQUINT binding)
	{
		switch (Renderer::GetGraphicAPI())
		{
		case GraphicAPI::None:
		{AQ_CORE_ASSERT(false, "RenderAPI::NO GraphicAPI is supported！"); break; }
		case GraphicAPI::OpenGL:
		{return  AQGLUniformBuffer::Create(size, binding); }
		}
		AQ_CORE_ASSERT(false, "RenderAPI::Unknown GraphicAPI!");
		return nullptr;
	}

	AQRef<AQUniformBuffer> AQUniformBuffer::Create(const std::string name, AQUINT size, AQUINT binding)
	{
		switch (Renderer::GetGraphicAPI())
		{
		case GraphicAPI::None:
		{AQ_CORE_ASSERT(false, "RenderAPI::NO GraphicAPI is supported！"); break; }
		case GraphicAPI::OpenGL:
		{return  AQGLUniformBuffer::Create(name, size, binding); }
		}
		AQ_CORE_ASSERT(false, "RenderAPI::Unknown GraphicAPI!");
		return nullptr;
	}
	
}
