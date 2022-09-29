#pragma once
#include "core/AquariusCore.h"
#include <string>
#include <GLAD/glad.h>


namespace Aquarius
{

	class AQUARIUS_API AQGLTexture2D
	{
	public:
		AQGLTexture2D(const char*& filename, GLsizei colorType = GL_RGB, GLsizei textureUnit = GL_TEXTURE0);
		AQGLTexture2D(const unsigned char* data, GLsizei width, GLsizei height, GLsizei colorType = GL_RGB, GLsizei textureUnit = GL_TEXTURE0);
		AQGLTexture2D(const AQGLTexture2D& other) = delete;
		AQGLTexture2D(AQGLTexture2D&& other) noexcept;

		~AQGLTexture2D();

		AQGLTexture2D& operator = (const AQGLTexture2D& other) = delete;
		AQGLTexture2D& operator = (AQGLTexture2D&& other)noexcept;

		GLuint& GetTexture() { return m_Texture; }
		void Bind();
		void UnBind();
		void Delete();
		void GenerateImage(GLsizei width, GLsizei height, GLsizei colorType, const unsigned char* data);
		void SetAttributes(GLsizei WRAP_S_ATTR = GL_REPEAT, GLsizei WRAP_T_ATTR = GL_REPEAT, GLsizei MIN_FILTER_ATTR = GL_LINEAR, GLsizei MAG_FILTER_ATTR = GL_LINEAR);
		void BindTextureUnit(GLsizei textureUnit);

		static void Mipmap(GLsizei textureUnit);
	private:


		GLuint m_Texture;

	};




}


