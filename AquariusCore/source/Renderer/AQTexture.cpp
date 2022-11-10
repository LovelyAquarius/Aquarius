#include "AQPCH.h"
#include "AQTexture.h"
#include "Platform/AQOpenGL/AQGLTexture.h"
#include "Renderer.h"

namespace Aquarius
{
	AQRef<AQTexture2D> AQTexture2D::Create(const std::string name ,const char* filepath)
	{
		switch (Renderer::GetGraphicAPI())
		{
		case GraphicAPI::None:
		{AQ_CORE_ASSERT(false, "RenderAPI::NO GraphicAPI is supported！"); break; }
		case GraphicAPI::OpenGL:
			return AQGLTexture2D::Create(name, filepath);
		}
		AQ_CORE_ASSERT(false, "RenderAPI::Unknown GraphicAPI!");
		return nullptr;
	}

	AQRef<AQTexture2D> AQTexture2D::Create(const char* filepath)
	{
		switch (Renderer::GetGraphicAPI())
		{
		case GraphicAPI::None:
		{AQ_CORE_ASSERT(false, "RenderAPI::NO GraphicAPI is supported！"); break; }
		case GraphicAPI::OpenGL:
			return AQGLTexture2D::Create(filepath);
		}
		AQ_CORE_ASSERT(false, "RenderAPI::Unknown GraphicAPI!");
		return nullptr;
	}
	AQRef<AQTexture2D> AQTexture2D::Create(const std::string name, AQUINT width, AQUINT height)
	{
		switch (Renderer::GetGraphicAPI())
		{
		case GraphicAPI::None:
		{AQ_CORE_ASSERT(false, "RenderAPI::NO GraphicAPI is supported！"); break; }
		case GraphicAPI::OpenGL:
			return  AQGLTexture2D::Create(name,width, height);
		}
		AQ_CORE_ASSERT(false, "RenderAPI::Unknown GraphicAPI!");
		return nullptr;
	}
	AQRef<AQSubTexture2D> AQSubTexture2D::Create(const std::string name, const AQRef<AQTexture2D>& texture, const Eigen::Vector2f& subsize, const Eigen::Vector2f& subcoordindex)
	{
		
		switch (Renderer::GetGraphicAPI())
		{
		case GraphicAPI::None:
			{AQ_CORE_ASSERT(false, "RenderAPI::NO GraphicAPI is supported！"); break; }
		case GraphicAPI::OpenGL:
			{
			if (!AQRefCast<AQGLTexture2D>(texture))
			{
				AQ_CORE_WARN("AQSubTexture2D::Create:Failed to Create AQSubTexture2D object ,the parent texture is null or failed to convert.");
				return nullptr;
			}
			else
				return  AQGLSubTexture2D::Create(name, texture, subsize, subcoordindex);
			}
		}
		AQ_CORE_ASSERT(false, "RenderAPI::Unknown GraphicAPI!");
		return nullptr;
	}
	AQRef<AQSubTexture2D> AQSubTexture2D::Create(const AQRef<AQTexture2D>& texture, const Eigen::Vector2f& subsize, const Eigen::Vector2f& subcoordindex)
	{
		switch (Renderer::GetGraphicAPI())
		{
		case GraphicAPI::None:
		{AQ_CORE_ASSERT(false, "RenderAPI::NO GraphicAPI is supported！"); break; }
		case GraphicAPI::OpenGL:
			{
				if (!AQRefCast<AQGLTexture2D>(texture))
					{
					AQ_CORE_WARN("AQSubTexture2D::Create:Failed to Create AQSubTexture2D object ,the parent texture is null or failed to convert.");
						return nullptr;
					}
				else
					return  AQGLSubTexture2D::Create(texture, subsize, subcoordindex);
			}
		}
		AQ_CORE_ASSERT(false, "RenderAPI::Unknown GraphicAPI!");
		return nullptr;
	}
}
