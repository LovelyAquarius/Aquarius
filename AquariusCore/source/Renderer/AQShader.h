#pragma once
#include "core/AquariusCore.h"
#include "core/AQObject.h"

namespace Aquarius
{
	class Renderer;
	class AQGLShader; 
	enum class GraphicAPI;

	class AQUARIUS_API AQShader:public AQObject
	{
	public:
		AQShader() { m_type = AQObjectType::AQShder; };
		static AQreference<AQShader>  Create(const std::string& name ,const char* filepath);
		static AQreference<AQShader>  Create(const char* filepath);
		template<class T, class ...args> static AQreference<AQShader> Create(const std::string& name, T head, args... indexs);

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		virtual void Delete()const = 0;

	};


	class AQUARIUS_API AQShaderLibrary
	{

	public:
		AQreference<AQShader>& Get(const std::string name);
	
		void Add(const std::string& name, const AQreference<AQShader>& shader);
		void Add(const AQreference<AQShader>& shader);
		AQreference<AQShader> Load(const char* filepath);
		AQreference<AQShader> Load(const std::string& name, const char* filepath);
		const bool DoesExist(const std::string& name);
	private:
		//lib中的名字和shader对象的名字是分别独立的。
		std::unordered_map<std::string, AQreference<AQShader>> m_ShaderLib;
		static AQreference<AQShader> empty;

	};











	template<class T, class ...args>
	 AQreference<AQShader> AQShader::Create(const std::string& name, T head, args ...indexs)
	{
		switch (Renderer::GetGraphicAPI())
		{
		case GraphicAPI::None:
		{AQ_CORE_ASSERT(false, "RenderAPI::NO GraphicAPI is supported！"); break; }
		case GraphicAPI::OpenGL:
			return new AQGLShader(name, head, indexs...);
		}
		AQ_CORE_ASSERT(false, "RenderAPI::Unknown GraphicAPI!");
		return AQreference<AQShader>();
	}


}