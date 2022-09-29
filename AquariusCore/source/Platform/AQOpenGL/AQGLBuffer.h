#include <GLAD/glad.h>
#include "Renderer/AQBuffer.h"
#include "Renderer/AQBufferLayout.h"

namespace Aquarius
{


	

	class AQUARIUS_API AQGLVertexBuffer : public AQVertexBuffer
	{
	public:
		friend class AQGLVertexArray;
		AQGLVertexBuffer(int datasize, const void* data, int datahandledtype);
		AQGLVertexBuffer(int datasize, const void* data, int datahandledtype,  AQGLVertexArray& parent);
		~AQGLVertexBuffer();

		AQGLVertexBuffer(AQGLVertexBuffer&) = delete;
		AQGLVertexBuffer(AQGLVertexBuffer&& other)noexcept;

		AQGLVertexBuffer& operator = (AQGLVertexBuffer& other) = delete;
		AQGLVertexBuffer& operator = (AQGLVertexBuffer&& other)noexcept;



		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual void Delete()const override;
		
		AQGLVertexArray*& GetParent();

	private:
		void CalculateBufferElementCount(int& datasize);
		void SetParentAttribute();
	private:

		AQGLVertexArray* m_Parent;
	};



	class AQUARIUS_API AQGLElementBuffer:public AQElementBuffer
	{
	public:
		friend class AQGLVertexArray;
		AQGLElementBuffer(int datasize, const void* data, int datahandledtype);
		AQGLElementBuffer(int datasize, const void* data, int datahandledtype , AQGLVertexArray& parent);
		~AQGLElementBuffer();

		AQGLElementBuffer(AQGLElementBuffer&) = delete;
		AQGLElementBuffer& operator =(AQGLElementBuffer&)=delete;


		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual void Delete()const override;
		
	private:

		AQGLVertexArray* m_Parent;
	};



	class AQUARIUS_API AQGLVertexArray:public AQVertexArray
	{
	public:
		AQGLVertexArray();
		AQGLVertexArray(AQBufferLayout& layout);
		AQGLVertexArray(AQGLVertexArray&) = delete;
		AQGLVertexArray(AQGLVertexArray&& other)noexcept;


		~AQGLVertexArray();


		void SetAttribute(GLsizei location, GLsizei vecsize, GLsizei datatype, GLsizei normalized, GLsizei stride, const void* offset);
		void SetAttribute(GLsizei location, GLsizei vecsize, GLsizei datatype, GLsizei normalized, GLsizei stride, const void* offset, AQGLVertexBuffer& correspond);//check version
		void SetAttribute(GLsizei location, GLsizei vecsize, GLsizei datatype, GLsizei normalized, GLsizei stride, const void* offset, AQGLElementBuffer& correspond);//check version

		//存在布局数据时不需要手动设置属性
		//调用前保证已绑定对应的VBO
		bool SetAttributeFromLayout();


		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual void Delete()const override;



	private:
		GLuint  m_VAO;
		
	};







	








}


