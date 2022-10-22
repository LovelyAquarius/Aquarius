#pragma once
#include "core/AquariusCore.h"
#include "core/AQObject.h"
#include <GLM/glm.hpp>
namespace Aquarius
{
	class Renderer;
	class AQGLShader; 
	enum class GraphicAPI;

	class AQUARIUS_API AQShader:public AQObject
	{
	public:
		static AQRef<AQShader>  Create(const std::string& name ,const char* filepath);
		static AQRef<AQShader>  Create(const char* filepath);
		template<class T, class ...args> static AQRef<AQShader> Create(const std::string& name, T head, args... indexs);
		static AQObjectType ClassType() { return AQObjectType::AQShader; }
	public:
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		virtual void Delete()const = 0;

		virtual void SetValue(const std::string& name, int value) const = 0;
		virtual void SetValue(const std::string& name, int* values, uint32_t count) const = 0;
		virtual void SetValue(const std::string& name, float value) const=0;
		virtual void SetValue(const std::string& name, float x, float y, float z) const=0;
		virtual void SetValue(const std::string& name, float x, float y, float z, float w) const=0;
		virtual void SetValue(const std::string& name, const glm::vec3& value)const=0;
		virtual void SetValue(const std::string& name, const glm::vec4& value)const=0;
		virtual void SetValue(const std::string& name, const glm::mat4& value)const=0;

	protected:
		AQShader() { m_type = AQObjectType::AQShader; }
		AQShader(const AQShader& other) { m_Name = other.m_Name; m_type = other.m_type; }
	};


	class AQUARIUS_API AQShaderLibrary
	{

	public:
		AQRef<AQShader>& Get(const std::string name);
	
		void Add(const std::string& name, const AQRef<AQShader>& shader);
		void Add(const AQRef<AQShader>& shader);
		AQRef<AQShader> Load(const char* filepath);
		AQRef<AQShader> Load(const std::string& name, const char* filepath);
		const bool DoesExist(const std::string& name);
	private:
		//lib中的名字和shader对象的名字是分别独立的。
		std::unordered_map<std::string, AQRef<AQShader>> m_ShaderLib;
		static AQRef<AQShader> empty;

	};











	template<class T, class ...args>
	 AQRef<AQShader> AQShader::Create(const std::string& name, T head, args ...indexs)
	{
		switch (Renderer::GetGraphicAPI())
		{
		case GraphicAPI::None:
		{AQ_CORE_ASSERT(false, "RenderAPI::NO GraphicAPI is supported！"); break; }
		case GraphicAPI::OpenGL:
			return  AQGLShader::Create(name, head, indexs...);
		}
		AQ_CORE_ASSERT(false, "RenderAPI::Unknown GraphicAPI!");
		return AQRef<AQShader>();
	}


}