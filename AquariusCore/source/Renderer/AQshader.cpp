#include "AQPCH.h"
#include "AQShader.h"
#include "Platform/AQOpenGL/AQGLShader.h"
#include "Renderer/Renderer.h"

namespace Aquarius
{

	AQRef<AQShader> AQShader::Create(const std::string& name, const char* filepath)
	{
		switch (Renderer::GetGraphicAPI())
		{
		case GraphicAPI::None:
		{AQ_CORE_ASSERT(false, "RenderAPI::NO GraphicAPI is supported！"); break; }
		case GraphicAPI::OpenGL:
			return AQGLShader::Create(name, filepath);
		}
		AQ_CORE_ASSERT(false, "RenderAPI::Unknown GraphicAPI!");
		return AQRef<AQShader>();
	}

	AQRef<AQShader> AQShader::Create(const char* filepath)
	{
		switch (Renderer::GetGraphicAPI())
		{
		case GraphicAPI::None:
		{AQ_CORE_ASSERT(false, "RenderAPI::NO GraphicAPI is supported！"); break; }
		case GraphicAPI::OpenGL:
			return  AQGLShader::Create(filepath);
		}
		AQ_CORE_ASSERT(false, "RenderAPI::Unknown GraphicAPI!");
		return AQRef<AQShader>();
	}

}
namespace Aquarius
{
	AQRef<AQShader> AQShaderLibrary::empty = AQRef<AQShader>();

	 AQRef<AQShader>& AQShaderLibrary::Get(const std::string name)
	{
		if (!DoesExist(name))
		{
			AQ_CORE_ERROR("Failed to get shader from library:the name'{0}' of the shader does not exist !", name);
			return AQShaderLibrary::empty;
		}
		else
			return m_ShaderLib[name];
	}


	void AQShaderLibrary::Add(const std::string& name, const AQRef<AQShader>& shader)
	{
		if (DoesExist(name))
			AQ_CORE_ERROR("Failed to add shader to library:the name'{0}' of the shader already exists !", name);
		else
			m_ShaderLib[name] = shader;
	}

	void AQShaderLibrary::Add(const AQRef<AQShader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	AQRef<AQShader> AQShaderLibrary::Load(const char* filepath)
	{
		auto shader = AQShader::Create(filepath);
		Add(shader);
		return shader;
	}

	AQRef<AQShader> AQShaderLibrary::Load(const std::string& name, const char* filepath)
	{
		auto shader = AQShader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	const bool AQShaderLibrary::DoesExist(const std::string& name)
	{
		return m_ShaderLib.find(name) != m_ShaderLib.end();
	}


}