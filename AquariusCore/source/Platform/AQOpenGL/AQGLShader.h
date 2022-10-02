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
		AQGLShader(const std::string& Name,const char* filepath);
		AQGLShader(const char* filepath);
		//多文件参数
		template<class T, class ...args> AQGLShader(const std::string& Name, T head, args... indexs);
		
		AQGLShader(AQGLShader& other);
		AQGLShader(AQGLShader&& other)noexcept;
		AQGLShader& operator= (AQGLShader& other);
		AQGLShader& operator= (AQGLShader&& other)noexcept;

		~AQGLShader();
		const GLuint& GetShaderProgram();

		virtual void Bind()const override;
		virtual void UnBind()const override;
		virtual void Delete()const override;

	public:
		//输入着色器方法
		void SetUniformVar(const std::string& name, int value) const;
		void SetUniformVar(const std::string& name, float value) const;
		void SetUniformVar(const std::string& name, float x, float y, float z) const;
		void SetUniformVar(const std::string& name, float x, float y, float z, float w) const;
		void SetUniformVar(const std::string& name, const glm::vec3& value);
		void SetUniformVar(const std::string& name, const glm::vec4& value);
		void SetUniformVar(const std::string& name, const glm::mat4& mat);

		

	private:
		std::string LoadFile(const std::string& filepath);
		std::unordered_map<GLenum,std::string> ParseCode(const std::string& rawcodes);
		GLenum ConvertGLShaderTypeFromString(const std::string& type);
		void Compile(const std::unordered_map<GLenum, std::string>& ripecodes);
		
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





	enum class vectortype
	{
		vec1 = 1, vec2 = 2, vec3 = 3, vec4 = 4
	};




}




