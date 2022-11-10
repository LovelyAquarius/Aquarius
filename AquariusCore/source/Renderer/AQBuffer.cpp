#pragma once
#include "AQPCH.h"

#include "AQBuffer.h"
#include "Platform/AQOpenGL/AQGLBuffer.h"
#include "Renderer.h"

namespace Aquarius
{
	AQRef<AQVertexBuffer> AQVertexBuffer::Create(AQUINT datasize, const std::string name)
	{
		switch (Renderer::GetGraphicAPI())
		{
		case GraphicAPI::None:
		{AQ_CORE_ASSERT(false, "RenderAPI::NO GraphicAPI is supported！"); break; }
		case GraphicAPI::OpenGL:
		{return  AQGLVertexBuffer::Create(datasize, name); }
		}
		AQ_CORE_ASSERT(false, "RenderAPI::Unknown GraphicAPI!");
		return AQRef<AQVertexBuffer>();
	}
	AQRef<AQVertexBuffer> AQVertexBuffer::Create(AQUINT datasize, const void* data, AQINT datahandledtype, const std::string name)
	{
		switch (Renderer::GetGraphicAPI())
		{
		case GraphicAPI::None:
		{AQ_CORE_ASSERT(false, "RenderAPI::NO GraphicAPI is supported！"); break; }
		case GraphicAPI::OpenGL:
		{return  AQGLVertexBuffer::Create(datasize, data, datahandledtype, name); }
		}
		AQ_CORE_ASSERT(false, "RenderAPI::Unknown GraphicAPI!");
		return AQRef<AQVertexBuffer>();
	}
	AQRef<AQVertexBuffer> AQVertexBuffer::Create(const AQBufferLayout& layout, AQUINT datasize, const void* data, AQINT datahandledtype, const std::string name)
	{
		switch (Renderer::GetGraphicAPI())
		{
		case GraphicAPI::None:
		{AQ_CORE_ASSERT(false, "RenderAPI::NO GraphicAPI is supported！"); break; }
		case GraphicAPI::OpenGL:
			return  AQGLVertexBuffer::Create(layout, datasize, data, datahandledtype,name);
		}
		AQ_CORE_ASSERT(false, "RenderAPI::Unknown GraphicAPI!");
		return AQRef<AQVertexBuffer>();
	}

	AQRef<AQElementBuffer> AQElementBuffer::Create(AQUINT elementcount, const void* data, AQINT datahandledtype, const std::string name)
	{
		switch (Renderer::GetGraphicAPI())
		{
		case GraphicAPI::None:
		{AQ_CORE_ASSERT(false, "RenderAPI::NO GraphicAPI is supported！"); break; }
		case GraphicAPI::OpenGL:
			return  AQGLElementBuffer::Create(elementcount, data, datahandledtype, name);
		}
		AQ_CORE_ASSERT(false, "RenderAPI::Unknown GraphicAPI!");
		return AQRef<AQElementBuffer>();
	}

	

	AQRef<AQVertexArray>  AQVertexArray::Create(const std::string name)
	{
		switch (Renderer::GetGraphicAPI())
		{
		case GraphicAPI::None:
		{AQ_CORE_ASSERT(false, "RenderAPI::NO GraphicAPI is supported!"); break; }
		case GraphicAPI::OpenGL:
			return  AQGLVertexArray::Create(name);
		}
		AQ_CORE_ASSERT(false, "RenderAPI::Unknown GraphicAPI!");
		return AQRef<AQVertexArray>();
	}




}
