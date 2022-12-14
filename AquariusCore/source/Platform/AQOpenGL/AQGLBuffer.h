#include <GLAD/glad.h>
#include "Renderer/AQBuffer.h"
#include "Renderer/AQBufferLayout.h"

namespace Aquarius
{


	

	class AQUARIUS_API AQGLVertexBuffer : public AQVertexBuffer
	{
	public:
		static AQRef<AQVertexBuffer> Create(AQUINT datasize, const void* data, AQINT datahandledtype, const std::string& name);
		static AQRef<AQVertexBuffer> Create(const AQBufferLayout& layout, AQUINT datasize, const void* data, AQINT datahandledtype, const std::string& name);
		static AQRef<AQVertexBuffer> Create(AQUINT datasize, const std::string& name);
		static AQObjectType ClassType() { return AQObjectType::AQGLVertexBuffer; }
	public:
		friend class AQGLVertexArray;
		~AQGLVertexBuffer();
		AQGLVertexBuffer(AQGLVertexBuffer&) = delete;
		GLuint Get()const { return m_VBO; }

		
		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual void Delete()const override;
		virtual void SetData(const void* data, AQUINT datasize)override;

		virtual  const AQBufferLayout& GetLayout()const { return m_Layout; }
		virtual   AQBufferLayout& GetLayout(){ return m_Layout; }
		virtual void SetLayout(const AQBufferLayout& layout);

	protected:
		AQGLVertexBuffer(const AQBufferLayout& layout, AQUINT datasize, const void* data, AQINT datahandledtype, const std::string& name);
		AQGLVertexBuffer(AQUINT datasize, const std::string& name);
		AQGLVertexBuffer(AQUINT datasize, const void* data, AQINT datahandledtype, const std::string& name);
	private:
		GLuint  m_VBO;
		GLint m_Buffersize;
		AQBufferLayout m_Layout;
	};



	class AQUARIUS_API AQGLElementBuffer:public AQElementBuffer
	{
	public:
		static  AQRef<AQElementBuffer>  Create(AQUINT elementcount, const void* data, AQINT datahandledtype, const std::string& name );
		static AQObjectType ClassType() { return AQObjectType::AQGLElementBuffer; }
	public:
		friend class AQGLVertexArray;
		
		~AQGLElementBuffer();
		AQGLElementBuffer(AQGLElementBuffer&) = delete;
		const GLuint& Get()const { return m_EBO; }
		const size_t& GetElementCount()const { return m_elementcount; }


		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual void Delete()const override;
		
	protected:
		AQGLElementBuffer(AQUINT elementcount, const void* data, AQINT datahandledtype, const std::string& name);
	private:
		GLuint  m_EBO;
		size_t m_elementcount;
	};



	class AQUARIUS_API AQGLVertexArray:public AQVertexArray
	{
	public:
		static AQRef<AQVertexArray> Create(const std::string& name );
		static AQObjectType ClassType() { return AQObjectType::AQGLVertexArray; }
	public:
		GLuint Get()const { return m_VAO; }

		~AQGLVertexArray();
		AQGLVertexArray(AQGLVertexArray&) = delete;
		void LinkVertexBuffer(AQRef<AQGLVertexBuffer> vbo);
		void LinkElementBuffer(AQRef<AQGLElementBuffer> ebo);
		virtual void LinkVE(AQRef<AQVertexBuffer> vbo, AQRef<AQElementBuffer> ebo)override;
		const AQRef<AQGLElementBuffer> GetLinkedEBO()const {return m_LinkedEBO;}

		void SetAttribute(GLsizei location, GLsizei vecsize, GLsizei datatype, GLsizei normalized, GLsizei stride, const void* offset);
		bool SetAttributeFromLayout();


		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual void Delete()const override;

		template<class T, class ...args>void Enable(T head, args... indexs);
		void Enable() {};

	protected:
		AQGLVertexArray(const std::string& name);

	private:
		GLuint  m_VAO;
		AQRef<AQGLVertexBuffer> m_LinkedVBO;
		AQRef<AQGLElementBuffer> m_LinkedEBO;
	};

	
	template<class T, class ...args>
	void AQGLVertexArray::Enable(T head, args... indexs)
	{
			GLCALL(glEnableVertexAttribArray(head));
			AQGLVertexArray::Enable(indexs...);
	}





}


