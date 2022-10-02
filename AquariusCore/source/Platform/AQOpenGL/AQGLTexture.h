#pragma once
#include "core/AquariusCore.h"
#include "Renderer/AQTexture.h"
#include <string>
#include <GLAD/glad.h>


namespace Aquarius
{

	class AQUARIUS_API AQGLTexture2D :public AQTexture2D
	{
	public:
		AQGLTexture2D(const char* filename);
		AQGLTexture2D();
		AQGLTexture2D(const AQGLTexture2D& other) = delete;
		AQGLTexture2D(AQGLTexture2D&& other) noexcept;

		~AQGLTexture2D();

		AQGLTexture2D& operator = (const AQGLTexture2D& other) = delete;
		AQGLTexture2D& operator = (AQGLTexture2D&& other)noexcept;

		GLuint& GetTexture() { return m_Texture; }
	    const GLsizei GetWidth()const override{ return m_Width; }
		const GLsizei  GetHeight() const { return m_Height; }

		void Bind();
		void UnBind();
		void Delete();
		void BindTextureUnit(GLsizei textureUnit);

		void SetAttributes(GLsizei WRAP_S_ATTR = GL_REPEAT, GLsizei WRAP_T_ATTR = GL_REPEAT, GLsizei MIN_FILTER_ATTR = GL_LINEAR, GLsizei MAG_FILTER_ATTR = GL_LINEAR);
		//手动创建纹理时，先调用SetAttributes方法告知怎么处理数据.
		void Load(const char* filename);
		
		static void Mipmap(GLsizei textureUnit);
		
	private:


		GLuint m_Texture;
		GLsizei m_Width=0;
		GLsizei m_Height=0;
		GLsizei m_nrChannels=0;
	};




}


