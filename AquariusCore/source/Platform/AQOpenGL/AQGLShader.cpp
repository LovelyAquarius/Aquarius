#include "AQPCH.h"
#include "core/AQCommon.h"
#include "AQGLShader.h"
#include "GLError.h"


namespace Aquarius
{
	AQRef<AQShader> AQGLShader::Create(const std::string& name, const char* filepath)
	{
		return new AQGLShader(name, filepath);
	}

	AQRef<AQShader> AQGLShader::Create(const char* filepath)
	{
		return new AQGLShader(filepath);
	}





	AQGLShader::AQGLShader(const std::string& Name, const char* filepath)
	{
		m_type = AQObjectType::AQGLShader;
		m_Name = Name;
		std::string rawcodes = LoadFile(filepath);
		auto ripecodes = ParseCode(rawcodes);
		Compile(ripecodes);
	}

	AQGLShader::AQGLShader(const char* filepath)
	{
		m_type = AQObjectType::AQGLShader;
		m_Name = AQ_ExtractFilename(filepath);
		std::string rawcodes = LoadFile(filepath);
		auto ripecodes = ParseCode(rawcodes);
		Compile(ripecodes);

	}





	AQGLShader::~AQGLShader()
	{
		if(m_ShaderProgram)
			GLCALL(glDeleteProgram(m_ShaderProgram));
	}

	void AQGLShader:: Delete()const
	{
		GLCALL(glDeleteProgram(m_ShaderProgram));
	}

	void AQGLShader::SetValue(const std::string& name, int value) const
	{
		SetUniformVar(name, value);
	}

	void AQGLShader::SetValue(const std::string& name, int* values, uint32_t count) const
	{
		SetUniformVar(name, values, count);
	}

	void AQGLShader::SetValue(const std::string& name, float value) const
	{
		SetUniformVar(name, value);
	}

	void AQGLShader::SetValue(const std::string& name, float x, float y, float z) const
	{
		SetUniformVar(name, x, y,z);
	}

	void AQGLShader::SetValue(const std::string& name, float x, float y, float z, float w) const
	{
		SetUniformVar(name, x, y, z, w);
	}

	void AQGLShader::SetValue(const std::string& name, const glm::vec3& value) const
	{
		SetUniformVar(name, value);
	}

	void AQGLShader::SetValue(const std::string& name, const glm::vec4& value) const
	{
		SetUniformVar(name, value);
	}

	void AQGLShader::SetValue(const std::string& name, const glm::mat4& value) const
	{
		SetUniformVar(name, value);
	}

	std::string AQGLShader::LoadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			AQ_CORE_ERROR("Failed to open the file:{0}", filepath);
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> AQGLShader::ParseCode(const std::string& rawcodes)
	{
		std::unordered_map<GLenum, std::string> ripecodes;
		const char* typetoken = "#TYPE";
		size_t typetokenlength = strlen(typetoken);
		size_t pos = rawcodes.find(typetoken, 0);

		while (pos != std::string::npos)
		{
			size_t EOL = rawcodes.find_first_of("\r\n", pos);
			AQ_CORE_ASSERT(EOL != std::string::npos, "Syntax error!");
			size_t codebegin = pos + typetokenlength + 1;
			std::string shadertype = rawcodes.substr(codebegin, EOL - codebegin);
			AQ_CORE_ASSERT(shadertype == "VERTEX" || shadertype=="GEOMETRY" || shadertype == "FRAGMENT" || shadertype == "PIXEL", "Invalid ShaderType:{0} specified!", shadertype);
			size_t nextline = rawcodes.find_first_not_of("\r\n", EOL);
			pos = rawcodes.find(typetoken, nextline);
			ripecodes[ConvertGLShaderTypeFromString(shadertype)] = rawcodes.substr(nextline, pos - (nextline == std::string::npos ? rawcodes.size() - 1 : nextline));
		}
		return ripecodes;
	}

	GLenum AQGLShader::ConvertGLShaderTypeFromString(const std::string& type)
	{
		if (type == "VERTEX") return GL_VERTEX_SHADER;
		else if (type == "FRAGMENT" || type == "PIXEL") return GL_FRAGMENT_SHADER;
		else if (type == "GEOMETRY") return GL_GEOMETRY_SHADER;
		AQ_CORE_ASSERT(false, "Invalid ShaderType:{0} specified!", type);
		return 0;
	}

	void AQGLShader::Compile(const std::unordered_map<GLenum, std::string>& ripecodes)
	{
		GLuint program;
		std::vector<GLenum> shaderids;
		GLCALL(program = glCreateProgram());
		for (auto& codekeyvalue : ripecodes)
		{
			//编译着色器
			GLenum shadertype = codekeyvalue.first;
			const GLchar* shadercode = codekeyvalue.second.c_str();
			GLuint shader;
			GLCALL(shader = glCreateShader(shadertype));
			GLCALL(glShaderSource(shader, 1, &shadercode, NULL));
			GLCALL(glCompileShader(shader));
			//_____________________________
			//编译错误日志
			GLint IsCompiled = 0;
			GLCALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &IsCompiled));
			if (IsCompiled == GL_FALSE)
			{
				GLint loglength = 0;
				GLCALL(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &loglength));
				std::vector<GLchar> log(loglength);
				GLCALL(glGetShaderInfoLog(shader, loglength, &loglength, &log[0]));
				GLCALL(glDeleteShader(shader));
				AQ_CORE_ERROR("SHADER:{0}(id:{1}):failed to compile! errorlog:\n{2}", m_Name, shader, log.data());
				AQ_CORE_ASSERT(false, "Failed to compile shader!", m_Name, shader, log.data());
			}
			//___________________________
			//添加子着色器
			GLCALL(glAttachShader(program, shader));
			shaderids.push_back(shader);
			//___________________________
		}
		//链接着色器程序
		GLCALL(glLinkProgram(program));
		//____________________________
		//链接错误日志
		GLint IsLinked = 0;
		GLCALL(glGetProgramiv(program, GL_LINK_STATUS, &IsLinked));
		if (IsLinked == GL_FALSE)
		{
			GLint loglength = 0;
			GLCALL(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &loglength));
			std::vector<GLchar> log(loglength);
			GLCALL(glGetProgramInfoLog(program, loglength, &loglength, &log[0]));
			for (auto id : shaderids)
			{
				GLCALL(glDeleteShader(id));
			}
			GLCALL(glDeleteProgram(program));
			AQ_CORE_ERROR("SHADERPROGRAM:{0}:failed to link shaders to program! errorlog:\n{1}", m_ShaderProgram, log.data());
			AQ_CORE_ASSERT(false, "SHADERPROGRAM:{0}:failed to link shaders to program! errorlog:\n{1}", m_ShaderProgram, log.data());
		}
		//_______________________________
		//程序制作完成
		for (auto id : shaderids)
		{
			GLCALL(glDetachShader(program, id));
		}
		m_ShaderProgram = program;
		//_______________________________
	}

	

	void AQGLShader::Bind()const
	{
		GLCALL(glUseProgram(m_ShaderProgram));
	}

	void AQGLShader::UnBind()const
	{
		GLCALL(glUseProgram(0));
	}

	void AQGLShader::SetUniformVar(const std::string& name, int value) const
	{
		GLCALL(glUniform1i(glGetUniformLocation(m_ShaderProgram, name.c_str()), value));
	}

	void AQGLShader::SetUniformVar(const std::string& name, int* values, uint32_t count) const
	{
		GLCALL(glUniform1iv(glGetUniformLocation(m_ShaderProgram, name.c_str()), count,values));
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

	void AQGLShader::SetUniformVar(const std::string& name, const glm::vec3& value)const
	{
		GLCALL(glUniform3f(glGetUniformLocation(m_ShaderProgram, name.c_str()), value.x, value.y, value.z));
	}

	void AQGLShader::SetUniformVar(const std::string& name, const glm::vec4& value)const
	{
		GLCALL(glUniform4f(glGetUniformLocation(m_ShaderProgram, name.c_str()),value.x, value.y, value.z, value.w));
	}



	void AQGLShader::SetUniformVar(const std::string& name, const glm::mat4& value)const
	{
		GLCALL(glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(value)));
	}


	const GLuint& AQGLShader::GetShaderProgram()
	{
		return m_ShaderProgram;
	}



}

