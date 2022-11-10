#pragma once
#include "AQEntity.h"
#include "core/TimeSystem.h"
namespace Aquarius
{
	class AQUARIUS_API AQScriptableEntity
	{
		friend class AQScene;
	public:
		template<typename ComponentType>
		AQRef<ComponentType> GetComponent();
	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy(){}
		virtual void OnUpdate(DeltaTime dt) {}
	private:
		AQEntity m_Entity;
	};

	template<typename ComponentType>
	AQRef<ComponentType> AQScriptableEntity::GetComponent()
	{
		return m_Entity.GetComponent<ComponentType>();
	}


}