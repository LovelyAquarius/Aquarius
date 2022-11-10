#pragma once
#include "core/AquariusCore.h"
#include "AQScene.h"
#include "entt/entt.hpp"

namespace Aquarius
{
	class AQUARIUS_API AQEntity
	{
	public:
		AQEntity() = default;
		AQEntity(entt::entity& handle, AQRef<AQScene>& scene);
		AQEntity(const AQEntity& other)=default;
	public:
		//所有类型的组件均以AQRef的形式传入，ComponentType为组件类型（ComponentType不是Ref类型）
		template<typename ComponentType, typename... Args>	 AQRef<ComponentType> AddComponent(Args&&... args);
		template<typename ComponentType>	void RemoveComponent();
		template<typename ComponentType>	AQRef<ComponentType> GetComponent();
		template<typename ComponentType>	bool HasComponent();
	public:
		operator bool()const {return m_EntityHandle != entt::null;}
		operator AQUINT() const { return (AQUINT)m_EntityHandle; }
		AQBOOL operator ==(const AQEntity& other)const {return m_Scene != nullptr&&m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;}
		AQBOOL operator != (const AQEntity& other)const { return m_Scene == nullptr || m_EntityHandle != other.m_EntityHandle || m_Scene != other.m_Scene;}
	private:
		entt::entity m_EntityHandle{entt::null};
		AQScene* m_Scene=nullptr;//这里不用引用计数，它不能参与控制scene的生存，后期可以改进引用机制。

	};



	template<typename ComponentType, typename ...Args>
	AQRef<ComponentType> AQEntity::AddComponent(Args&&... args)
	{
		if (!m_Scene)
		{
			AQ_CORE_ASSERT(false, "AQEntity::AddComponent:Failed to addcomponent, this entiy does not belong to any scene or the scene belonged to has been destroyed!(entityhandle:{0})", (AQUINT)m_EntityHandle);
			AQ_CORE_ERROR("AQEntity::AddComponent:Failed to addcomponent, this entiy does not belong to any scene or the scene belonged to has been destroyed!(entityhandle:{0})", (AQUINT)m_EntityHandle);
			return AQRef<ComponentType>();
		}
		else
		{
			return m_Scene->m_Registry.emplace<AQRef<ComponentType>>(m_EntityHandle, ComponentType::Create(std::forward<Args>(args)...));
		}
	}

	template<typename ComponentType>
	 void AQEntity::RemoveComponent()
	{
		 if (!m_Scene)
		 {
			 AQ_CORE_ASSERT(false, "AQEntity::AddComponent:Failed to addcomponent, this entiy does not belong to any scene or the scene belonged to has been destroyed!(entityhandle:{0})", (AQUINT)m_EntityHandle);
			 AQ_CORE_ERROR("AQEntity::GetComponent:Failed to getcomponent, this entiy does not belong to any scene or the scene belonged to has been destroyed!(entityhandle:{0})", (AQUINT)m_EntityHandle);
			 return;
		 }
		 else if (!HasComponent<ComponentType>())
		 {
			 AQ_CORE_ASSERT(false, "AQEntity::GetComponent:Failed to getcomponent, this entiy does not has this type of component!(entityhandle:{0})", (AQUINT)m_EntityHandle);
			 AQ_CORE_ERROR("AQEntity::GetComponent:Failed to getcomponent, this entiy does not has this type of component!(entityhandle:{0})", (AQUINT)m_EntityHandle);
			 return;
		 }
		 else
			 m_Scene->m_Registry.remove<AQRef<ComponentType>>(m_EntityHandle);
	}

	template<typename ComponentType>
	AQRef<ComponentType> AQEntity::GetComponent()
	{
		if (!m_Scene)
		{
			AQ_CORE_ASSERT(false, "AQEntity::AddComponent:Failed to addcomponent, this entiy does not belong to any scene or the scene belonged to has been destroyed!(entityhandle:{0})", (AQUINT)m_EntityHandle);
			AQ_CORE_ERROR("AQEntity::GetComponent:Failed to getcomponent, this entiy does not belong to any scene or the scene belonged to has been destroyed!(entityhandle:{0})", (AQUINT)m_EntityHandle);
			return AQRef<ComponentType>();
		}
		else if (!HasComponent<ComponentType>())
		{
			AQ_CORE_ASSERT(false, "AQEntity::GetComponent:Failed to getcomponent, this entiy does not has this type of component!(entityhandle:{0})", (AQUINT)m_EntityHandle);
			AQ_CORE_ERROR("AQEntity::GetComponent:Failed to getcomponent, this entiy does not has this type of component!(entityhandle:{0})", (AQUINT)m_EntityHandle);
			return AQRef<ComponentType>();
		}
		else
			return m_Scene->m_Registry.get<AQRef<ComponentType>>(m_EntityHandle);
	}

	template<typename ComponentType>
	bool AQEntity::HasComponent()
	{
		if (!m_Scene)
		{
			AQ_CORE_ASSERT(false, "AQEntity::AddComponent:Failed to addcomponent, this entiy does not belong to any scene or the scene belonged to has been destroyed!(entityhandle:{0})", (AQUINT)m_EntityHandle);
			AQ_CORE_ERROR("AQEntity::GetComponent:Failed to getcomponent, this entiy does not belong to any scene or the scene belonged to has been destroyed!(entityhandle:{0})", (AQUINT)m_EntityHandle);
			return false;
		}
		else
			return m_Scene->m_Registry.any_of<AQRef<ComponentType>>(m_EntityHandle);
	}
}


