#pragma once
#include "core/AquariusCore.h"
#include "Renderer/AQUniformBuffer.h"


namespace Aquarius
{
	class AQGLUniformBuffer :public AQUniformBuffer
	{
	public:
		static AQRef<AQUniformBuffer> Create(AQUINT size, AQUINT binding);
		static AQRef<AQUniformBuffer> Create(const std::string& name, AQUINT size, AQUINT binding);

	public:
		virtual ~AQGLUniformBuffer();
		virtual void SetData(const void* data, AQUINT size, AQUINT offset = 0) override;


	protected:
		AQGLUniformBuffer(AQUINT size, AQUINT binding);
		AQGLUniformBuffer(const std::string& name, AQUINT size, AQUINT binding);
	private:
		AQUINT m_UBO = 0;
	};
}


