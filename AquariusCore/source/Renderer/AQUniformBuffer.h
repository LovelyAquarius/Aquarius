#pragma once
#include "core/AquariusCore.h"
#include "core/AQObject.h"

namespace Aquarius
{
	class AQUARIUS_API AQUniformBuffer:public AQObject
	{
	public:
		static AQRef<AQUniformBuffer> Create(AQUINT size, AQUINT binding);
		static AQRef<AQUniformBuffer> Create(const std::string name, AQUINT size, AQUINT binding);
		virtual ~AQUniformBuffer() {}
		virtual void SetData(const void* data, AQUINT size, AQUINT offset = 0) = 0;
	
	protected:
		AQUniformBuffer() { m_type = AQObjectType::AQUniformBuffer; }
	};







}


