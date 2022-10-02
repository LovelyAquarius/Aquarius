#include "AQPCH.h"
#include "AQGLBuffer.h"
#include "GLError.h"

namespace Aquarius

{
	AQGLVertexBuffer::AQGLVertexBuffer(int datasize, const void* data, int datahandledtype)
		:m_Parent(nullptr)
	{
		m_type = AQObjectType::AQGLVertexBuffer;
		GLCALL(glGenBuffers(1, &m_VBO));
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
		GLCALL(glBufferData(GL_ARRAY_BUFFER, datasize, data, datahandledtype));
	}


	AQGLVertexBuffer::AQGLVertexBuffer(int datasize, const void* data, int datahandledtype, AQGLVertexArray& parent)
		:m_Parent(&parent)
	{
		m_type = AQObjectType::AQGLVertexBuffer;
		GLCALL(glGenBuffers(1, &m_VBO));
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
		GLCALL(glBufferData(GL_ARRAY_BUFFER, datasize, data, datahandledtype));
		CalculateBufferElementCount(datasize);
		SetParentAttribute();
	}

	

	AQGLVertexBuffer::~AQGLVertexBuffer()
	{
		if (m_VBO)
			GLCALL(glDeleteBuffers(1, &m_VBO));
	}

	AQGLVertexBuffer::AQGLVertexBuffer(AQGLVertexBuffer&& other)noexcept
		:m_Parent(other.m_Parent)
	{
		m_VBO = other.m_VBO;
		other.m_VBO = 0;
	}



	 void AQGLVertexBuffer::Delete()const
	{
		 GLCALL(glDeleteBuffers(1, &m_VBO));
	}
	 AQGLVertexBuffer& AQGLVertexBuffer::operator=(AQGLVertexBuffer&& other) noexcept
	 {
		 m_VBO = other.m_VBO;
		 m_Parent = other.m_Parent;
		 other.m_VBO = 0;
		 return *this;
	 }
	 void AQGLVertexBuffer::Bind() const
	{
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
	}

	void AQGLVertexBuffer::UnBind() const
	{
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	


	AQGLVertexArray*& AQGLVertexBuffer::GetParent()
	{
		return m_Parent;
	}

	void AQGLVertexBuffer::CalculateBufferElementCount(int& datasize)
	{
		if (m_Parent->GetElementNumber())
		{
			m_Parent->GetLayout().CalculateCount(datasize);
		}
		else
		{
			AQ_CORE_ERROR("VAOID:{0} has no AQBufferLayout!", m_Parent->Get());
		}
	}

	void AQGLVertexBuffer::SetParentAttribute()
	{
		if (m_Parent->GetElementNumber())
		{
			m_Parent->Bind();
			m_Parent->SetAttributeFromLayout();
		}
		else
		{
			AQ_CORE_ERROR("VAOID:{0} has no AQBufferLayout!", m_Parent->Get());
		}
	}





	AQGLVertexArray::AQGLVertexArray()
		:m_VAO(1)
	{
		m_type = AQObjectType::AQGLVertexArray;
		GLCALL(glGenVertexArrays(1, &m_VAO));
		GLCALL(glBindVertexArray(m_VAO));
	}

	AQGLVertexArray::AQGLVertexArray(AQBufferLayout& layout)
		:m_VAO(1)
	{
		m_type = AQObjectType::AQGLVertexArray;
		GLCALL(glGenVertexArrays(1, &m_VAO));
		GLCALL(glBindVertexArray(m_VAO));
		SetLayout(layout);
		
	}
	AQGLVertexArray::AQGLVertexArray(AQGLVertexArray&& other) noexcept
		:m_VAO(other.m_VAO)
	{
		m_Layout = other.m_Layout;
		ElementNumber = other.ElementNumber;
		other.m_VAO = 0;
	}
	void AQGLVertexArray::Delete()const
	{

		GLCALL(glDeleteBuffers(1, &m_VAO));
	}
	AQGLVertexArray::~AQGLVertexArray()
	{
		if(m_VAO)
			GLCALL(glDeleteBuffers(1, &m_VAO));
	}

	

	void AQGLVertexArray::SetAttribute(GLsizei location, GLsizei vecsize, GLsizei datatype, GLsizei normalized, GLsizei stride, const void* offset)
	{
		GLCALL(glVertexAttribPointer(location, vecsize, datatype, normalized, stride, offset));
		ElementNumber++;
	}

	void AQGLVertexArray::SetAttribute(GLsizei location, GLsizei vecsize, GLsizei datatype, GLsizei normalized, GLsizei stride, const void* offset, AQGLVertexBuffer& correspond)
	{
		if (this == correspond.m_Parent)
		{
			GLCALL(glVertexAttribPointer(location, vecsize, datatype, normalized, stride, offset));
			ElementNumber++;
		}
		else
			AQ_CORE_WARN("VAO SetAttribute failed:The Attribute which is setting is not correspond to specified VBO!");
	}

	void AQGLVertexArray::SetAttribute(GLsizei location, GLsizei vecsize, GLsizei datatype, GLsizei normalized, GLsizei stride, const void* offset, AQGLElementBuffer& correspond)
	{
		if (this == correspond.m_Parent)
		{
			GLCALL(glVertexAttribPointer(location, vecsize, datatype, normalized, stride, offset));
			ElementNumber++;
		}
		else
			AQ_CORE_WARN("VAO SetAttribute failed:The Attribute which is setting is not correspond to specified EBO!");
	}

	bool AQGLVertexArray::SetAttributeFromLayout()
	{
		ElementNumber = (unsigned int)m_Layout.GetElements().size();

		if (ElementNumber ==0)
		{
			AQ_CORE_WARN("Layout data is empty");
			return false;
		}
		else
		{
			for (auto& element : m_Layout.GetElements())
			{
				GLCALL(glVertexAttribPointer(element.BufferLocation, element.GetDimension(), element.GetGLType(), element.Normalized, m_Layout.GetStride(), (void*)element.Offset));
			}

		}



	}



	void AQGLVertexArray::Bind()const
	{
		GLCALL(glBindVertexArray(m_VAO));
	}

	void AQGLVertexArray::UnBind()const
	{
		GLCALL(glBindVertexArray(0));
	}



	AQGLElementBuffer::AQGLElementBuffer(int datasize, const void* data, int datahandledtype)
		:m_Parent(nullptr)
	{
		m_type = AQObjectType::AQGLElementBuffer;
		GLCALL(glGenBuffers(1, &m_EBO));
		GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO));
		GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, datasize, data, datahandledtype));

	}

	AQGLElementBuffer::AQGLElementBuffer(int datasize, const void* data,  int datahandledtype, AQGLVertexArray& parent)
		:m_Parent(&parent)
	{
		m_type = AQObjectType::AQGLElementBuffer;
		GLCALL(glGenBuffers(1, &m_EBO));
		GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO));
		GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, datasize, data, datahandledtype));

		if (m_Parent->GetElementNumber())
			m_Parent->GetLayout().SetIndex(datasize / sizeof(int));
	}

	AQGLElementBuffer::~AQGLElementBuffer()
	{
	}
	void AQGLElementBuffer::Delete()const
	{
		GLCALL(glDeleteBuffers(1, &m_EBO));
	}
	void AQGLElementBuffer::Bind() const
	{
		GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO));
	}

	void AQGLElementBuffer::UnBind() const
	{
		GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}



}