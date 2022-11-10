#include "AQPCH.h"
#include "AQGLBuffer.h"
#include "GLError.h"

namespace Aquarius

{
	AQGLVertexBuffer::AQGLVertexBuffer(const AQBufferLayout& layout, AQUINT datasize, const void* data, AQINT datahandledtype, const std::string& name)
		:m_VBO(0), m_Buffersize(datasize)
	{
		if (name.size())
			m_Name = name;
		else
			m_Name = "Unnamed AQGLVertexBuffer";
		m_type = AQObjectType::AQGLVertexBuffer;
		GLCALL(glGenBuffers(1, &m_VBO));
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
		GLCALL(glBufferData(GL_ARRAY_BUFFER, datasize, data, datahandledtype));
		SetLayout(layout);
	}
	AQGLVertexBuffer::AQGLVertexBuffer(AQUINT datasize, const std::string& name)
		:m_VBO(0), m_Buffersize(datasize)
	{
		if (name.size())
			m_Name = name;
		else
			m_Name = "Unnamed AQGLVertexBuffer";
		m_type = AQObjectType::AQGLVertexBuffer;
		GLCALL(glGenBuffers(1, &m_VBO));
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
		GLCALL(glBufferData(GL_ARRAY_BUFFER, datasize, nullptr, GL_DYNAMIC_DRAW));

	}
	AQGLVertexBuffer::AQGLVertexBuffer(AQUINT datasize, const void* data, AQINT datahandledtype, const std::string& name)
		:m_VBO(0), m_Buffersize(datasize)
	{
		if (name.size())
			m_Name = name;
		else
			m_Name = "Unnamed AQGLVertexBuffer";
		m_type = AQObjectType::AQGLVertexBuffer;
		GLCALL(glGenBuffers(1, &m_VBO));
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
		GLCALL(glBufferData(GL_ARRAY_BUFFER, datasize, data, datahandledtype));
	}


	AQRef<AQVertexBuffer> AQGLVertexBuffer::Create(AQUINT datasize, const void* data, AQINT datahandledtype, const std::string& name)
	{
		return new AQGLVertexBuffer(datasize, data, datahandledtype, name);
	}

	AQRef<AQVertexBuffer> AQGLVertexBuffer::Create(const AQBufferLayout& layout, AQUINT datasize, const void* data, AQINT datahandledtype, const std::string& name)
	{
		return new AQGLVertexBuffer(layout, datasize, data, datahandledtype, name);
	}

	AQRef<AQVertexBuffer> AQGLVertexBuffer::Create(AQUINT datasize, const std::string& name)
	{
		return new AQGLVertexBuffer(datasize, name);
	}

	AQGLVertexBuffer::~AQGLVertexBuffer()
	{
		if (m_VBO)
			GLCALL(glDeleteBuffers(1, &m_VBO));
	}


	void AQGLVertexBuffer::Bind() const
	{
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
	}

	void AQGLVertexBuffer::UnBind() const
	{
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void AQGLVertexBuffer::Delete() const
	{
		if (m_VBO)
			GLCALL(glDeleteBuffers(1, &m_VBO));
	}

	void AQGLVertexBuffer::SetData(const void* data, AQUINT datasize)
	{

		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
		GLCALL(glBufferSubData(GL_ARRAY_BUFFER, 0, datasize, data));
	}




	void AQGLVertexBuffer::SetLayout(const AQBufferLayout& layout)
	{
		m_Layout = layout;
		m_Layout.CalculateCount(m_Buffersize);
	}

}

namespace Aquarius
{

	AQRef<AQVertexArray> AQGLVertexArray::Create(const std::string& name)
	{
		return new AQGLVertexArray(name);
	}



	AQGLVertexArray::AQGLVertexArray(const std::string& name)
		:m_VAO(0)
	{
		if (name.size())
			m_Name = name;
		else
			m_Name = "Unnamed AQGLVertexArray";
		m_type = AQObjectType::AQGLVertexArray;
		GLCALL(glGenVertexArrays(1, &m_VAO));
		GLCALL(glBindVertexArray(m_VAO));
	}



	AQGLVertexArray::~AQGLVertexArray()
	{
		if (m_VAO)
			GLCALL(glDeleteBuffers(1, &m_VAO));
	}

	void AQGLVertexArray::Bind()const
	{
		GLCALL(glBindVertexArray(m_VAO));
	}

	void AQGLVertexArray::UnBind()const
	{
		GLCALL(glBindVertexArray(0));
	}

	void AQGLVertexArray::Delete()const
	{
		if (m_VAO)
			GLCALL(glDeleteBuffers(1, &m_VAO));
	}


	void AQGLVertexArray::SetAttribute(GLsizei location, GLsizei vecsize, GLsizei datatype, GLsizei normalized, GLsizei stride, const void* offset)
	{
		GLCALL(glBindVertexArray(m_VAO));
		GLCALL(glVertexAttribPointer(location, vecsize, datatype, normalized, stride, offset));
	}


	bool AQGLVertexArray::SetAttributeFromLayout()
	{
		size_t ElementNumber = m_LinkedVBO->GetLayout().GetElements().size();
		if (ElementNumber == 0)
		{
			AQ_CORE_WARN("Layout data is empty");
			return false;
		}
		else
		{
			GLCALL(glBindVertexArray(m_VAO));
			for (auto& element : m_LinkedVBO->GetLayout().GetElements())
			{
				GLCALL(glVertexAttribPointer(element.BufferLocation, element.GetDimension(), element.GetGLType(), element.Normalized, m_LinkedVBO->GetLayout().GetStride(), (void*)element.Offset));
			}
			return true;
		}
	}

	void AQGLVertexArray::LinkVertexBuffer(AQRef<AQGLVertexBuffer> vbo)
	{
		m_LinkedVBO = vbo;
		GLCALL(glBindVertexArray(m_VAO));
		m_LinkedVBO->Bind();
		SetAttributeFromLayout();
	}

	void AQGLVertexArray::LinkElementBuffer(AQRef<AQGLElementBuffer> ebo)
	{
		m_LinkedEBO = ebo;
		GLCALL(glBindVertexArray(m_VAO));
		m_LinkedEBO->Bind();
	}
	void AQGLVertexArray::LinkVE(AQRef<AQVertexBuffer> vbo, AQRef<AQElementBuffer> ebo)
	{
		GLCALL(glBindVertexArray(m_VAO));
		m_LinkedVBO = AQRefCast<AQGLVertexBuffer>(vbo);
		m_LinkedVBO->Bind();
		SetAttributeFromLayout();
		m_LinkedEBO = AQRefCast<AQGLElementBuffer>(ebo);
		m_LinkedEBO->Bind();
	}
}


namespace Aquarius
{
	AQGLElementBuffer::AQGLElementBuffer(AQUINT elementcount, const void* data, int datahandledtype, const std::string& name)
	{
		if (name.size())
			m_Name = name;
		else
			m_Name = "Unnamed AQGLElementBuffer";
		m_type = AQObjectType::AQGLElementBuffer;
		m_elementcount = elementcount;
		GLCALL(glGenBuffers(1, &m_EBO));
		GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO));
		GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_elementcount*sizeof(int), data, datahandledtype));
		
	}


	AQRef<AQElementBuffer> AQGLElementBuffer::Create(AQUINT elementcount, const void* data, int datahandledtype, const std::string& name)
	{
		return new AQGLElementBuffer(elementcount, data, datahandledtype, name);
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