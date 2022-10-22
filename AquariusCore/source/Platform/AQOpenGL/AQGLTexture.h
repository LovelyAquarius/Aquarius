#pragma once
#include "core/AquariusCore.h"
#include "core/AQCommon.h"
#include "Renderer/AQTexture.h"
#include <string>
#include <GLAD/glad.h>


namespace Aquarius
{

	class AQUARIUS_API AQGLTexture2D :public AQTexture2D
	{
	public:
		static AQRef<AQTexture2D> Create(const std::string name, const char* filepath);
		static AQRef<AQTexture2D>  Create(const char* filepath);
		static AQRef<AQTexture2D>  Create(const std::string name, uint32_t width, uint32_t height);
		static AQObjectType ClassType() { return AQObjectType::AQGLTexture2D; }
	public:
		
		~AQGLTexture2D();
		virtual bool operator==( AQRef<AQTexture2D>& other)const override{return m_Texture == AQRefCast<AQGLTexture2D>(other)->m_Texture;}

		virtual const unsigned int  GetTextureID()const override{ return m_Texture; }
	    const GLsizei GetWidth()const override{ return m_Width; }
		const GLsizei  GetHeight() const { return m_Height; }

		virtual void Bind()const override;
		virtual void UnBind()const override;
		virtual void Delete()const override;
		virtual void BindSlot(GLuint textureslot)override;

		void SetAttributes(GLsizei MIN_FILTER_ATTR = GL_LINEAR, GLsizei MAG_FILTER_ATTR = GL_LINEAR,GLsizei WRAP_S_ATTR = GL_REPEAT, GLsizei WRAP_T_ATTR = GL_REPEAT);
		//手动创建纹理时，先调用SetAttributes方法告知怎么处理数据.
		void LoadFile(const char* filepath)override;
		void LoadData(void* data, uint32_t datasize)override;


	protected:
		AQGLTexture2D(const char* filepath, const std::string& name);
		AQGLTexture2D(const char* filepath);
		AQGLTexture2D(const std::string& name, uint32_t width, uint32_t height);
	private:


		GLuint m_Texture;
		GLsizei m_Width=0;
		GLsizei m_Height=0;
		GLsizei m_nrChannels=0;
		GLenum m_Internalformat = 0;
		GLenum m_Dataformat = 0;
	};


	class AQUARIUS_API AQGLSubTexture2D :public AQSubTexture2D
	{
	public:
		static AQRef<AQSubTexture2D> Create(const std::string& name,const AQRef<AQGLTexture2D>& texture, const glm::vec2& subsize, const glm::vec2& subcoordindex);
		static AQRef<AQSubTexture2D> Create(const AQRef<AQGLTexture2D>& texture, const glm::vec2& subsize, const glm::vec2& subcoordindex);

		virtual AQRef<AQObject> Copy();
		virtual const AQRef<AQTexture2D> GetTexture() const override{ return AQRefCast<AQTexture2D>(m_ParentTexture); }
		virtual const glm::vec2* GetTexCoords() const override { return m_TexCoords; }

	protected:
		AQGLSubTexture2D(const AQRef<AQGLTexture2D>& texture,const glm::vec2& minimumbound, const glm::vec2& maximumbound);
		AQGLSubTexture2D(const std::string& name, const AQRef<AQGLTexture2D>& texture, const glm::vec2& minimumbound, const glm::vec2& maximumbound);
		AQGLSubTexture2D(const AQGLSubTexture2D& other);
	private:
		AQRef<AQGLTexture2D> m_ParentTexture;

		glm::vec2 m_TexCoords[4];
	};

}


