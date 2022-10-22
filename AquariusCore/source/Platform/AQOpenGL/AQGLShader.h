#pragma once
#include "core/AquariusCore.h"
#include "Renderer/AQShader.h"
#include <GLAD/glad.h>

//glm数学依赖项
#include<GLM/vec2.hpp>
#include <GLM/vec2.hpp>
#include <GLM/glm.hpp>
#include <GLM/ext/matrix_clip_space.hpp>
#include <GLM/gtc/type_ptr.hpp>
//___________________-


namespace Aquarius
{
	class AQUARIUS_API AQGLShader : public AQShader
	{
	public:
		static AQRef<AQShader>  Create(const std::string& name, const char* filepath);
		static AQRef<AQShader>  Create(const char* filepath);
		template<class T, class ...args> static AQRef<AQShader> Create(const std::string& name, T head, args... indexs);
		static AQObjectType ClassType() { return AQObjectType::AQGLShader; }
	public:
		
		~AQGLShader();
		const GLuint& GetShaderProgram();

		virtual void Bind()const override;
		virtual void UnBind()const override;
		virtual void Delete()const override;

		virtual void SetValue(const std::string& name, int value) const override;
		virtual void SetValue(const std::string& name, int* values, uint32_t count) const override;
		virtual void SetValue(const std::string& name, float value) const override;
		virtual void SetValue(const std::string& name, float x, float y, float z) const override;
		virtual void SetValue(const std::string& name, float x, float y, float z, float w) const override;
		virtual void SetValue(const std::string& name, const glm::vec3& value)const override;
		virtual void SetValue(const std::string& name, const glm::vec4& value)const override;
		virtual void SetValue(const std::string& name, const glm::mat4& value)const override;







	private:
		//输入着色器方法
		void SetUniformVar(const std::string& name, int value) const;
		void SetUniformVar(const std::string& name, int* values, uint32_t count) const;
		void SetUniformVar(const std::string& name, float value) const;
		void SetUniformVar(const std::string& name, float x, float y, float z) const;
		void SetUniformVar(const std::string& name, float x, float y, float z, float w) const;
		void SetUniformVar(const std::string& name, const glm::vec3& value)const;
		void SetUniformVar(const std::string& name, const glm::vec4& value)const;
		void SetUniformVar(const std::string& name, const glm::mat4& value)const;

		

	private:
		std::string LoadFile(const std::string& filepath);
		std::unordered_map<GLenum,std::string> ParseCode(const std::string& rawcodes);
		GLenum ConvertGLShaderTypeFromString(const std::string& type);
		void Compile(const std::unordered_map<GLenum, std::string>& ripecodes);
		
	protected:
		AQGLShader(const std::string& Name, const char* filepath);
		AQGLShader(const char* filepath);
		//多文件参数
		template<class T, class ...args> AQGLShader(const std::string& Name, T head, args... indexs);
	private:
		GLuint m_ShaderProgram=0;

	};

	template<class T, class ...args>
	AQGLShader::AQGLShader(const std::string& Name, T head, args... indexs)
	{
		m_type = AQObjectType::AQGLShder;
		m_Name = Name;
		std::string rawcodes; 
		rawcodes.append(LoadFile(head));
		rawcodes.append("\n"+ LoadFile(indexs...));
		auto ripecodes = ParseCode(rawcodes);
		Compile(ripecodes);
	}

	template<class T, class ...args>
	AQRef<AQShader> AQGLShader::Create(const std::string& name, T head, args ...indexs)
	{
		return new AQGLShader(name, head, indexs...);
	}



}






