#include "AQPCH.h"

#include "AQGLTexture.h"
#include "core/AQCommon.h"
#include "GLError.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>


namespace Aquarius

{
	AQRef<AQTexture2D> AQGLTexture2D::Create(const std::string& name, const char* filepath)
	{
		return new AQGLTexture2D(filepath, name);
	}

	AQRef<AQTexture2D> AQGLTexture2D::Create(const char* filepath)
	{
		return new AQGLTexture2D(filepath);
	}

	AQRef<AQTexture2D> AQGLTexture2D::Create(const std::string& name, AQUINT width, AQUINT height)
	{
		return new AQGLTexture2D(name, width, height);
	}

	AQGLTexture2D::AQGLTexture2D(const char* filepath, const std::string& name)
		:m_Texture(0)
	{
		if (name.size())
			m_Name = name;
		else
			m_Name = "Unnamed AQGLVertexArray";
		m_type = AQObjectType::AQGLTexture2D;
		GLCALL(glGenTextures(1, &m_Texture));
		SetAttributes();
		LoadFile(filepath);
	}

	AQGLTexture2D::AQGLTexture2D(const char* filepath)
		:m_Texture(0)
	{
		m_type = AQObjectType::AQGLTexture2D;
		GLCALL(glGenTextures(1, &m_Texture));
		SetAttributes();
		LoadFile(filepath);
		m_Name = AQ_ExtractFilename(filepath);
	}

	AQGLTexture2D::AQGLTexture2D(const std::string& name, AQUINT width, AQUINT height)
		:m_Width(width), m_Height(height)
	{
		if (name.size())
			m_Name = name;
		else
			m_Name = "Unnamed AQGLVertexArray";
		m_type = AQObjectType::AQGLTexture2D;
		m_Internalformat = GL_RGBA8;
		m_Dataformat = GL_RGBA;
		GLCALL(glGenTextures(1, &m_Texture));
		SetAttributes();
		GLCALL(glTextureStorage2D(m_Texture, 1, m_Internalformat, m_Width, m_Height));

	}







	AQGLTexture2D::~AQGLTexture2D()
	{
		if (m_Texture)
			GLCALL(glDeleteTextures(1, &m_Texture));
	}



	void AQGLTexture2D::Bind()const
	{
		GLCALL(glBindTexture(GL_TEXTURE_2D, m_Texture));
	}

	void AQGLTexture2D::UnBind()const
	{
		GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
	}

	void AQGLTexture2D::Delete()const
	{
		if (m_Texture)
			GLCALL(glDeleteTextures(1, &m_Texture));
	}


	void AQGLTexture2D::SetAttributes(GLsizei MIN_FILTER_ATTR, GLsizei MAG_FILTER_ATTR, GLsizei WRAP_S_ATTR, GLsizei WRAP_T_ATTR)
	{
		GLCALL(glBindTexture(GL_TEXTURE_2D, m_Texture));
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MIN_FILTER_ATTR));
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MAG_FILTER_ATTR));
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WRAP_S_ATTR));
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WRAP_T_ATTR));

	}

	void AQGLTexture2D::BindSlot(GLuint textureslot)
	{
		GLCALL(glBindTextureUnit(textureslot, m_Texture));
		//GLCALL(glActiveTexture(GL_TEXTURE0 + textureslot));
		//GLCALL(glBindTexture(GL_TEXTURE_2D, m_Texture));
	}

	void AQGLTexture2D::LoadFile(const char* filepath)
	{
		//读取数据
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(filepath, &m_Width, &m_Height, &m_nrChannels, 0);
		AQ_CORE_ASSERT(data, "Reading texture2d file has failed!");
		GLenum internalformat = 0;
		GLenum dataformat = 0;
		if (m_nrChannels == 3)
		{
			internalformat = GL_RGB8;
			dataformat = GL_RGB;
		}
		else if (m_nrChannels == 4)
		{
			internalformat = GL_RGBA8;
			dataformat = GL_RGBA;
		}
		AQ_CORE_ASSERT(internalformat & dataformat, "Unsuported channel type!");
		m_Internalformat = internalformat;
		m_Dataformat = dataformat;
		GLCALL(glBindTexture(GL_TEXTURE_2D, m_Texture));
		GLCALL(glTextureStorage2D(m_Texture, 1, internalformat, m_Width, m_Height));
		GLCALL(glTextureSubImage2D(m_Texture, 0, 0, 0, m_Width, m_Height, dataformat, GL_UNSIGNED_BYTE, data));
		stbi_image_free(data);
		//___________________________
	}

	void AQGLTexture2D::LoadData(void* data, AQUINT datasize)
	{
		uint32_t bytesperchannel = m_Dataformat == GL_RGBA ? 4 : 3;
		if (datasize == m_Width * m_Height * bytesperchannel)
		{
			GLCALL(glTextureSubImage2D(m_Texture, 0, 0, 0, m_Width, m_Height, m_Dataformat, GL_UNSIGNED_BYTE, data));
		}
		else
		{
			AQ_CORE_ERROR("Failed to load data for texture:{0}, datasize is wrong!", m_Name);
		}

	}

	void Mipmap(GLsizei textureUnit)
	{
		GLCALL(glActiveTexture(textureUnit));
		GLCALL(glGenerateMipmap(GL_TEXTURE_2D));
	}


}



namespace Aquarius
{
	AQRef<AQSubTexture2D> AQGLSubTexture2D::Create(const std::string& name, const AQRef<AQGLTexture2D>& texture, const Eigen::Vector2f& subsize, const Eigen::Vector2f& subcoordindex)
	{
		Eigen::Vector2f minimumbound = { (subcoordindex.x()) * subsize.x() / texture->GetWidth(),(subcoordindex.y()) * subsize.y() / texture->GetHeight()};
		Eigen::Vector2f maximumbound = { (subcoordindex.x() +1) * subsize.x() / texture->GetWidth(),(subcoordindex.y() +1) * subsize.y() / texture->GetHeight() };
		return new AQGLSubTexture2D(name, texture, minimumbound, maximumbound);
	}
	AQRef<AQSubTexture2D> AQGLSubTexture2D::Create(const AQRef<AQGLTexture2D>& texture, const Eigen::Vector2f& subsize, const Eigen::Vector2f& subcoordindex)
	{
		Eigen::Vector2f minimumbound = { (subcoordindex.x()) * subsize.x() / texture->GetWidth(),(subcoordindex.y()) * subsize.y() / texture->GetHeight() };
		Eigen::Vector2f maximumbound = { (subcoordindex.x() + 1) * subsize.x() / texture->GetWidth(),(subcoordindex.y() + 1) * subsize.y() / texture->GetHeight() };
		return new AQGLSubTexture2D(texture, minimumbound, maximumbound);
	}
	AQRef<AQObject> AQGLSubTexture2D::Copy()
	{
		Eigen::Vector2f minimumbound = m_TexCoords[0];
		Eigen::Vector2f maximumbound = m_TexCoords[2];
		return new AQGLSubTexture2D(m_Name,m_ParentTexture, minimumbound, maximumbound);
	}
	AQGLSubTexture2D::AQGLSubTexture2D(const AQRef<AQGLTexture2D>& texture, const Eigen::Vector2f& minimumbound, const Eigen::Vector2f& maximumbound)
		:m_ParentTexture(texture)
	{
		m_Name = "Unnamed AQGLSubTexture2D";
		m_type = AQObjectType::AQGLSubTexture2D;

		m_TexCoords[0] = { minimumbound.x(),minimumbound.y() };
		m_TexCoords[1] = { maximumbound.x(),minimumbound.y() };
		m_TexCoords[2] = { maximumbound.x(),maximumbound.y() };
		m_TexCoords[3] = { minimumbound.x(),maximumbound.y() };
	}

	AQGLSubTexture2D::AQGLSubTexture2D(const std::string& name, const AQRef<AQGLTexture2D>& texture, const Eigen::Vector2f& minimumbound, const Eigen::Vector2f& maximumbound)
		:m_ParentTexture (texture)
	{
		if (name.size())
			m_Name = name;
		else
			m_Name = "Unnamed AQGLSubTexture2D";
		m_type = AQObjectType::AQGLSubTexture2D;

		m_TexCoords[0] = { minimumbound.x(),minimumbound.y() };
		m_TexCoords[1]  = { maximumbound.x(),minimumbound.y() };
		m_TexCoords[2] = { maximumbound.x(),maximumbound.y() };
		m_TexCoords[3] = { minimumbound.x(),maximumbound.y() };
	}

	AQGLSubTexture2D::AQGLSubTexture2D(const AQGLSubTexture2D& other)
		:m_ParentTexture(other.GetTexture())
	{
		m_Name = other.GetName();
		m_type = other.GetType();

		m_TexCoords[0] = other.GetTexCoords()[0];
		m_TexCoords[1] = other.GetTexCoords()[1];
		m_TexCoords[2] = other.GetTexCoords()[2];
		m_TexCoords[3] = other.GetTexCoords()[3];
	}

}


