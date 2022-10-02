#pragma once
#include "AQPCH.h"

#include "AQBuffer.h"
#include "Platform/AQOpenGL/AQGLBuffer.h"
#include "Renderer.h"

namespace Aquarius
{

	AQRef<AQVertexBuffer> AQVertexBuffer::Create(int datasize, const void* data, int datahandledtype)
	{
		switch (Renderer::GetGraphicAPI())
		{
		case GraphicAPI::None:
		{AQ_CORE_ASSERT(false, "RenderAPI::NO GraphicAPI is supported！"); break; }
		case GraphicAPI::OpenGL:
			return new AQGLVertexBuffer(datasize, data, datahandledtype);
		}
		AQ_CORE_ASSERT(false, "RenderAPI::Unknown GraphicAPI!");
		return AQRef<AQVertexBuffer>();
	}

	AQRef<AQVertexBuffer> AQVertexBuffer::Create(int datasize, const void* data, int datahandledtype, AQVertexArray& parent)
	{
		switch (Renderer::GetGraphicAPI())
		{
		case GraphicAPI::None:
		{AQ_CORE_ASSERT(false, "RenderAPI::NO GraphicAPI is supported！"); break; }
		case GraphicAPI::OpenGL:
			return new AQGLVertexBuffer(datasize, data, datahandledtype, (AQGLVertexArray&)parent);
		}
		AQ_CORE_ASSERT(false, "RenderAPI::Unknown GraphicAPI!");
		return AQRef<AQVertexBuffer>();
	}

	AQRef<AQElementBuffer> AQElementBuffer::Create(int datasize, const void* data, int datahandledtype)
	{
		switch (Renderer::GetGraphicAPI())
		{
		case GraphicAPI::None:
		{AQ_CORE_ASSERT(false, "RenderAPI::NO GraphicAPI is supported！"); break; }
		case GraphicAPI::OpenGL:
			return new AQGLElementBuffer(datasize, data, datahandledtype);
		}
		AQ_CORE_ASSERT(false, "RenderAPI::Unknown GraphicAPI!");
		return AQRef<AQElementBuffer>();
	}

	AQRef<AQElementBuffer> AQElementBuffer::Create(int datasize, const void* data, int datahandledtype, AQVertexArray& parent)
	{
		switch (Renderer::GetGraphicAPI())
		{
		case GraphicAPI::None:
		{AQ_CORE_ASSERT(false, "RenderAPI::NO GraphicAPI is supported！"); break; }
		case GraphicAPI::OpenGL:
			return new AQGLElementBuffer(datasize, data, datahandledtype, (AQGLVertexArray&)parent);
		}
		AQ_CORE_ASSERT(false, "RenderAPI::Unknown GraphicAPI!");
		return AQRef<AQElementBuffer>();
	}

	AQRef<AQVertexArray>  AQVertexArray::Create(AQBufferLayout& layout)
	{
		switch (Renderer::GetGraphicAPI())
		{
		case GraphicAPI::None:
		{AQ_CORE_ASSERT(false, "RenderAPI::NO GraphicAPI is supported!"); break; }
		case GraphicAPI::OpenGL:
			return new AQGLVertexArray(layout);
		}
		AQ_CORE_ASSERT(false, "RenderAPI::Unknown GraphicAPI!");
		return AQRef<AQVertexArray>();
	}



	const AQBufferLayout& AQVertexArray::GetLayout() const
	{
		return m_Layout;
	}

	AQBufferLayout& AQVertexArray::GetLayout()
	{
		return m_Layout;
	}

	void AQVertexArray::SetLayout(AQBufferLayout& layout)
	{
		m_Layout = layout;
		ElementNumber = m_Layout.GetElements().size();
	}

	const GLuint& AQVertexArray::GetElementNumber() const
	{
		return ElementNumber;
	}






}
