#include "AQPCH.h"

#include "AQGLTexture.h"
#include "GLError.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>


namespace Aquarius

{

	AQGLTexture2D::AQGLTexture2D(const char* filename)
		:m_Texture(0)
	{
		GLCALL(glGenTextures(1, &m_Texture));
		SetAttributes();
		Load(filename);
	}

	AQGLTexture2D::AQGLTexture2D()
		:m_Texture(0)
	{
		GLCALL(glGenTextures(1, &m_Texture));
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

	void AQGLTexture2D::Load(const char* filename)
	{
		//读取数据
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(filename, &m_Width, &m_Height, &m_nrChannels, 0);
		AQ_CORE_ASSERT(data, "Reading texture2d file has failed!");
		GLenum internalformat = 0;
		GLenum dataformat = 0;
		if (m_nrChannels == 3)
		{
			internalformat = GL_RGB8;
			dataformat = GL_RGB;
		}
		else if(m_nrChannels==4)
		{
			internalformat = GL_RGBA8;
			dataformat = GL_RGBA;
		}
		AQ_CORE_ASSERT(internalformat & dataformat, "Unsuported channel type!");
		GLCALL(glBindTexture(GL_TEXTURE_2D, m_Texture));
		GLCALL(glTextureStorage2D(m_Texture, 1, internalformat, m_Width, m_Height));
		GLCALL(glTextureSubImage2D(m_Texture, 0, 0, 0, m_Width, m_Height, dataformat, GL_UNSIGNED_BYTE, data));
		stbi_image_free(data);
		//___________________________
	}

	void Mipmap(GLsizei textureUnit)
	{
		GLCALL(glActiveTexture(textureUnit));
		GLCALL(glGenerateMipmap(GL_TEXTURE_2D));
	}








}


