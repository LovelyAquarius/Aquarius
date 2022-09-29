#pragma once
#include "core/AquariusCore.h"

#include <string>
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
	class AQUARIUS_API AQGLShader
	{
	public:
		AQGLShader(const std::string& vertexshadersource, const std::string& fragmentshadersource);
		AQGLShader(const char* vertexPath, const char* fragmentPath);
		AQGLShader(AQGLShader& other);
		AQGLShader(AQGLShader&& other)noexcept;
		AQGLShader& operator= (AQGLShader& other);
		AQGLShader& operator= (AQGLShader&& other)noexcept;

		~AQGLShader();
		void Delete();
		void CreateShader(GLuint& shader, int shadertype, const GLchar* shadercode);
		void CreateShaderProgram(unsigned int& shaderprogram);

		void Use();
		void UnUse();
		void SetUniformVar(const std::string& name, int value) const;
		void SetUniformVar(const std::string& name, float value) const;
		void SetUniformVar(const std::string& name, float x, float y, float z) const;
		void SetUniformVar(const std::string& name, float x, float y, float z, float w) const;
		void SetUniformVar(const std::string& name, glm::mat4 mat);






		GLuint& GetShaderProgram();
		std::string GetVertexShaderSourceCode();
		std::string GetFragmentShaderSourceCode();



	private:

		GLuint m_VertexShader = 1;

	
		GLuint m_FragmentShader = 2;

	
		GLuint m_ShaderProgram;

		std::string* m_VertexShaderCode;
		std::string* m_FragmentShaderCode;

	};

	enum class vectortype
	{
		vec1 = 1, vec2 = 2, vec3 = 3, vec4 = 4
	};




}




