#include "AQPCH.h"
#include "AQGLShader.h"

#include "GLError.h"


namespace Aquarius
{

	AQGLShader::AQGLShader(const std::string& vertexshadercode, const std::string& fragmentshadercode)
		:m_VertexShaderCode(new std::string(vertexshadercode)),
		m_FragmentShaderCode(new std::string(fragmentshadercode))
	{
		const GLchar* vertexCode = (const GLchar*)m_VertexShaderCode->c_str();
		const GLchar* fragmentCode = (const GLchar*)m_FragmentShaderCode->c_str();
		CreateShader(m_VertexShader, GL_VERTEX_SHADER, vertexCode);
		CreateShader(m_FragmentShader, GL_FRAGMENT_SHADER, fragmentCode);
		CreateShaderProgram(m_ShaderProgram);
	}

	AQGLShader::AQGLShader(const char* vertexPath, const char* fragmentPath)
		:m_VertexShaderCode(new std::string()),
		m_FragmentShaderCode(new std::string())
	{
		
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			

			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			

			vShaderFile.close();
			fShaderFile.close();
		

			*m_VertexShaderCode = vShaderStream.str();
			*m_FragmentShaderCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		const GLchar* vertexCode = (const GLchar*)m_VertexShaderCode->c_str();
		const GLchar* fragmentCode = (const GLchar*)m_FragmentShaderCode->c_str();


		CreateShader(m_VertexShader, GL_VERTEX_SHADER, vertexCode);
		CreateShader(m_FragmentShader, GL_FRAGMENT_SHADER, fragmentCode);
		CreateShaderProgram(m_ShaderProgram);

	}

	AQGLShader::AQGLShader(AQGLShader& other)
		:m_VertexShaderCode(new std::string(*other.m_VertexShaderCode)),
		m_FragmentShaderCode(new std::string(*other.m_FragmentShaderCode)),
		m_VertexShader(other.m_VertexShader), m_FragmentShader(other.m_VertexShader),
		m_ShaderProgram(other.m_ShaderProgram ) {}

	
	AQGLShader::AQGLShader(AQGLShader&& other) noexcept
		:m_VertexShaderCode(std::move(other.m_VertexShaderCode)),
		m_FragmentShaderCode(std::move(other.m_FragmentShaderCode)),
		m_VertexShader(other.m_VertexShader), m_FragmentShader(other.m_VertexShader),
		m_ShaderProgram(other.m_ShaderProgram) 
	{
		other.m_FragmentShaderCode = nullptr;
		other.m_FragmentShaderCode = nullptr;

	}

	AQGLShader& AQGLShader::operator=(AQGLShader& other)
	{
		m_VertexShaderCode = new std::string(*other.m_VertexShaderCode);
		m_FragmentShaderCode = new std::string(*other.m_FragmentShaderCode);
		m_VertexShader = other.m_VertexShader;
		m_FragmentShader = other.m_FragmentShader;
		m_ShaderProgram = other.m_ShaderProgram;
		return *this;
	}

	AQGLShader& AQGLShader::operator=(AQGLShader&& other) noexcept
	{
		m_VertexShaderCode = std::move(other.m_VertexShaderCode);
		m_FragmentShaderCode = std::move(other.m_FragmentShaderCode);
		m_VertexShader = other.m_VertexShader;
		m_FragmentShader = other.m_FragmentShader;
		m_ShaderProgram = other.m_ShaderProgram;
		return *this;
	}

	AQGLShader::~AQGLShader()
	{
		delete m_VertexShaderCode;
		delete m_FragmentShaderCode;
	}

	void AQGLShader:: Delete()
	{
		GLCALL(glDeleteProgram(m_ShaderProgram));
	}



	void AQGLShader::CreateShader(GLuint& shader, int shadertype, const GLchar* shadercode)
	{

		switch (shadertype)
		{
		case GL_VERTEX_SHADER:
		{
			GLCALL(m_VertexShader = glCreateShader(shadertype));
			break;
		}
		case GL_FRAGMENT_SHADER:
		{
			GLCALL(m_FragmentShader = glCreateShader(shadertype));
			break;
		}

		}
		GLCALL(glShaderSource(shader, 1, &shadercode, NULL));
		GLCALL(glCompileShader(shader));


		int  success;
		char infoLog[512];
		GLCALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
		if (!success)
		{
			GLCALL(glGetShaderInfoLog(shader, 512, NULL, infoLog));
			std::cout << "ERROR::SHADER::" << std::hex << shader << "::COMPILATION_FAILED\n" << infoLog << std::endl;
		}



	}



	void AQGLShader::CreateShaderProgram(unsigned int& shaderprogram)
	{
		GLCALL(shaderprogram = glCreateProgram());
		GLCALL(glAttachShader(shaderprogram, m_VertexShader));
		GLCALL(glAttachShader(shaderprogram, m_FragmentShader));
		GLCALL(glLinkProgram(shaderprogram));


		int  success;
		char infoLog[512];
		GLCALL(glGetProgramiv(shaderprogram, GL_LINK_STATUS, &success));
		if (!success) {
			GLCALL(glGetProgramInfoLog(shaderprogram, 512, NULL, infoLog));
			std::cout << "ERROR::SHADERPROGRAM::CREATION_FAILED\n" << infoLog << std::endl;
		}


		GLCALL(glDeleteShader(m_VertexShader));
		GLCALL(glDeleteShader(m_FragmentShader));
		GLCALL(glDetachShader(shaderprogram, m_VertexShader));
		GLCALL(glDetachShader(shaderprogram, m_FragmentShader));

	}

	void AQGLShader::Use()
	{
		GLCALL(glUseProgram(m_ShaderProgram));

	}

	void AQGLShader::UnUse()
	{
		GLCALL(glUseProgram(0));
	}

	void AQGLShader::SetUniformVar(const std::string& name, int value) const
	{
		GLCALL(glUniform1i(glGetUniformLocation(m_ShaderProgram, name.c_str()), value));
	}

	void AQGLShader::SetUniformVar(const std::string& name, float value) const
	{
		GLCALL(glUniform1f(glGetUniformLocation(m_ShaderProgram, name.c_str()), value));
	}

	void AQGLShader::SetUniformVar(const std::string& name, float x, float y, float z) const
	{
		GLCALL(glUniform3f(glGetUniformLocation(m_ShaderProgram, name.c_str()), x, y, z));
	}

	void AQGLShader::SetUniformVar(const std::string& name, float x, float y, float z, float w) const
	{
		GLCALL(glUniform4f(glGetUniformLocation(m_ShaderProgram, name.c_str()), x, y, z, w));
	}

	void AQGLShader::SetUniformVar(const std::string& name, glm::mat4 mat)
	{
		GLCALL(glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat)));
	}


	GLuint& AQGLShader::GetShaderProgram()
	{
		return m_ShaderProgram;
	}

	std::string AQGLShader::GetVertexShaderSourceCode()
	{
		return std::string(*m_VertexShaderCode);
	}

	std::string AQGLShader::GetFragmentShaderSourceCode()
	{
		return std::string(*m_FragmentShaderCode);
	}



}

