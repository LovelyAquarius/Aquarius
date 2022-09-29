#pragma once
#include "core/AquariusCore.h"


#include "AQBufferLayout.h"
#include "Platform/AQOpenGL/GLError.h"


namespace Aquarius
{
	class AQVertexArray;
	class AQBufferLayout;
	class Renderer;
	enum class GraphicAPI;

	class AQUARIUS_API AQVertexBuffer
	{
	public:
		AQVertexBuffer() :m_VBO(0) {};

		static AQVertexBuffer* Create(int datasize, const void* data, int datahandledtype);
		static AQVertexBuffer* Create(int datasize, const void* data, int datahandledtype, AQVertexArray& parent);
		unsigned int& Get(){ return m_VBO; };

		virtual void Bind() const=0;
		virtual void UnBind() const=0;
		virtual void Delete()const=0;

	protected:
		unsigned int  m_VBO;

	};


	class AQUARIUS_API AQElementBuffer
	{
	public:
		AQElementBuffer() :m_EBO(0) {};
		static AQElementBuffer* Create(int datasize, const void* data, int datahandledtype);
		static AQElementBuffer* Create(int datasize, const void* data, int datahandledtype, AQVertexArray& parent);


		unsigned int& Get() { return m_EBO; };

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		virtual void Delete()const = 0;

	protected:
		unsigned int  m_EBO;

	};

	class AQUARIUS_API AQVertexArray
	{
	public:
		AQVertexArray() :m_VAO(0) ,ElementNumber (0) {};
		static AQVertexArray* Create(AQBufferLayout& layout);



		unsigned int& Get() { return m_VAO; };

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		virtual void Delete()const = 0;

		template<class T, class ...args>void Enable(T head, args... indexs);
		void Enable() {};




		virtual  const AQBufferLayout& GetLayout()const;
		virtual   AQBufferLayout& GetLayout();
		virtual void SetLayout(AQBufferLayout& layout);
		virtual  const GLuint& GetElementNumber()const;

	protected:
		unsigned int  m_VAO;
		//注意这里暂时另存了一份独立的layout 后续可能会修改成指针
		AQBufferLayout m_Layout;
		GLuint ElementNumber;
	};

	template<class T, class ...args>
	void AQVertexArray::Enable(T head, args... indexs)
	{
		switch (Renderer::GetGraphicAPI())
		{
		case GraphicAPI::None:
		{AQ_CORE_ASSERT(false, "RenderAPI::NO GraphicAPI is supported！"); break; }
		case GraphicAPI::OpenGL:
		{
			GLCALL(glEnableVertexAttribArray(head));
			AQVertexArray::Enable(indexs...);
			break;
		}
		}


	}


}