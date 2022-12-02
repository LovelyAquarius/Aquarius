#include "AQPCH.h"
#include "AQGLUniformBuffer.h"
#include "GLError.h"
#include <GLAD/glad.h>
namespace Aquarius
{
	AQRef<AQUniformBuffer> AQGLUniformBuffer::Create(AQUINT size, AQUINT binding)
	{
		return new AQGLUniformBuffer(size, binding);
	}

	AQRef<AQUniformBuffer> AQGLUniformBuffer::Create(const std::string& name, AQUINT size, AQUINT binding)
	{
		return new AQGLUniformBuffer(name, size, binding);
	}

	AQGLUniformBuffer::~AQGLUniformBuffer()
	{
		if(m_UBO)
			GLCALL(glDeleteBuffers(1, &m_UBO));
	}

	void AQGLUniformBuffer::SetData(const void* data, AQUINT size, AQUINT offset)
	{
		GLCALL(glNamedBufferSubData(m_UBO, offset, size, data));
	}

	AQGLUniformBuffer::AQGLUniformBuffer(AQUINT size, AQUINT binding)
	{
		AQ_INITIALIZE_AQOBJECT_TYPE(AQGLUniformBuffer);

		GLCALL(glCreateBuffers(1, &m_UBO));
		GLCALL(glNamedBufferData(m_UBO, size, nullptr, GL_DYNAMIC_DRAW));
		GLCALL(glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_UBO));
	}

	AQGLUniformBuffer::AQGLUniformBuffer(const std::string& name, AQUINT size, AQUINT binding)
	{
		AQ_INITIALIZE_AQOBJECT_NAME_AND_TYPE(name, AQGLUniformBuffer);

		GLCALL(glCreateBuffers(1, &m_UBO));
		GLCALL(glNamedBufferData(m_UBO, size, nullptr, GL_DYNAMIC_DRAW));
		GLCALL(glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_UBO));
	}


}