#include "AQPCH.h"
#include "AQTexture.h"
#include "Platform/AQOpenGL/AQGLTexture.h"
#include "Renderer.h"

namespace Aquarius
{
	AQTexture2D* AQTexture2D::Create(const char* filename)
	{
		switch (Renderer::GetGraphicAPI())
		{
		case GraphicAPI::None:
		{AQ_CORE_ASSERT(false, "RenderAPI::NO GraphicAPI is supported！"); break; }
		case GraphicAPI::OpenGL:
			return new AQGLTexture2D(filename);
		}
		AQ_CORE_ASSERT(false, "RenderAPI::Unknown GraphicAPI!");
		return nullptr;
	}

	AQTexture2D* AQTexture2D::Create()
	{
		switch (Renderer::GetGraphicAPI())
		{
		case GraphicAPI::None:
		{AQ_CORE_ASSERT(false, "RenderAPI::NO GraphicAPI is supported！"); break; }
		case GraphicAPI::OpenGL:
			return new AQGLTexture2D();
		}
		AQ_CORE_ASSERT(false, "RenderAPI::Unknown GraphicAPI!");
		return nullptr;
	}
}
