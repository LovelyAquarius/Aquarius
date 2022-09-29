#include "AQPCH.h"

#include "AQGLTexture.h"
#include "GLError.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>


namespace Aquarius

{

	AQGLTexture2D::AQGLTexture2D(const char*& filename, GLsizei colorType, GLsizei textureUnit)
		:m_Texture(1)
	{
		GLsizei width;
		GLsizei height;
		GLsizei nrChannels;

		GLCALL(glGenTextures(1, &m_Texture));
		GLCALL(glActiveTexture(textureUnit));
		SetAttributes();

		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
		AQ_CORE_ASSERT(data, "reading texture2d picture has failed!");


		GenerateImage(width, height, colorType, data);
		stbi_image_free(data);

	}

	AQGLTexture2D::AQGLTexture2D(const unsigned char* data, GLsizei width, GLsizei height, GLsizei colorType, GLsizei textureUnit)
		:m_Texture(1)

	{
		GLCALL(glGenTextures(1, &m_Texture));
		GLCALL(glActiveTexture(textureUnit));
		Bind();

		GenerateImage(width, height, colorType, data);
	}

	AQGLTexture2D::AQGLTexture2D(AQGLTexture2D&& other) noexcept
		:m_Texture(other.GetTexture())
	{
		other.GetTexture() = 0;
	}



	AQGLTexture2D::~AQGLTexture2D()
	{
		if(m_Texture)
			GLCALL(glDeleteTextures(1, &m_Texture));
	}

	AQGLTexture2D& AQGLTexture2D::operator=(AQGLTexture2D&& other)noexcept
	{
		m_Texture = other.GetTexture();
		other.GetTexture() = 0;
		return *this;
	}

	void AQGLTexture2D::Bind()
	{
		GLCALL(glBindTexture(GL_TEXTURE_2D, m_Texture));
	}

	void AQGLTexture2D::UnBind()
	{
		GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
	}

	void AQGLTexture2D::Delete()
	{
		GLCALL(glDeleteTextures(1, &m_Texture));
	}

	void AQGLTexture2D::GenerateImage(GLsizei width, GLsizei height, GLsizei colorType, const unsigned char* data)
	{
		GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, colorType, width, height, 0, colorType, GL_UNSIGNED_BYTE, data));
	}

	void AQGLTexture2D::SetAttributes(GLsizei WRAP_S_ATTR, GLsizei WRAP_T_ATTR, GLsizei MIN_FILTER_ATTR, GLsizei MAG_FILTER_ATTR)
	{
		GLCALL(glBindTexture(GL_TEXTURE_2D, m_Texture));
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WRAP_S_ATTR));
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WRAP_T_ATTR));
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MIN_FILTER_ATTR));
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MAG_FILTER_ATTR));

	}

	void AQGLTexture2D::BindTextureUnit(GLsizei textureUnit)
	{
		GLCALL(glActiveTexture(textureUnit));
		GLCALL(glBindTexture(GL_TEXTURE_2D, m_Texture));
	}

	void Mipmap(GLsizei textureUnit)
	{
		GLCALL(glActiveTexture(textureUnit));
		GLCALL(glGenerateMipmap(GL_TEXTURE_2D));
	}








}


