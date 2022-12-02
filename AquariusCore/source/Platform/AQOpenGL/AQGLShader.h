#pragma once
#include "Renderer/AQShader.h"
#include<GLAD/glad.h>




namespace Aquarius
{
	class  AQGLShader : public AQShader
	{
	public:
		static AQRef<AQShader>  Create(const std::string& name, const char* filepath);
		static AQRef<AQShader>  Create(const char* filepath);
		template<class T, class ...args> static AQRef<AQShader> Create(const std::string& name, T head, args... indexs);
		static AQObjectType ClassType() { return AQObjectType::AQGLShader; }
	public:
		
		~AQGLShader();
		AQGLShader(AQGLShader&) = delete;
		const GLuint& GetShaderProgram();

		virtual void Bind()const override;
		virtual void UnBind()const override;
		virtual void Delete()const override;
		virtual AQUINT GetId() const override { return m_ShaderProgram; };
		virtual void SetValue(const std::string& name, AQINT value) const override;
		virtual void SetValue(const std::string& name, AQINT* values, AQUINT count) const override;
		virtual void SetValue(const std::string& name, AQFLOAT value) const override;
		virtual void SetValue(const std::string& name, AQFLOAT x, AQFLOAT y, AQFLOAT z) const override;
		virtual void SetValue(const std::string& name, AQFLOAT x, AQFLOAT y, AQFLOAT z, AQFLOAT w) const override;
		virtual void SetValue(const std::string& name, const Eigen::Vector3f& value)const override;
		virtual void SetValue(const std::string& name, const Eigen::Vector4f& value)const override;
		virtual void SetValue(const std::string& name, const Eigen::Matrix4f& value)const override;







	private:
		//输入着色器方法
		void SetUniformVar(const std::string& name, AQINT value) const;
		void SetUniformVar(const std::string& name, AQINT* values, AQUINT count) const;
		void SetUniformVar(const std::string& name, AQFLOAT value) const;
		void SetUniformVar(const std::string& name, AQFLOAT x, AQFLOAT y, AQFLOAT z) const;
		void SetUniformVar(const std::string& name, AQFLOAT x, AQFLOAT y, AQFLOAT z, AQFLOAT w) const;
		void SetUniformVar(const std::string& name, const Eigen::Vector3f& value)const;
		void SetUniformVar(const std::string& name, const Eigen::Vector4f& value)const;
		void SetUniformVar(const std::string& name, const Eigen::Matrix4f& value)const;

		

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






