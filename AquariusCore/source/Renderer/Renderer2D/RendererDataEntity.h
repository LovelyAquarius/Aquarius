#pragma once
#include "core/AquariusCore.h"
#include "core/AQObject.h"
#include "entt/entt.hpp"


namespace Aquarius
{
	class AQUARIUS_API RendererDataEntity
	{
	public:
		RendererDataEntity() = default;
		RendererDataEntity(entt::entity& handle, entt::registry* rendererdata):m_EntityHandle(handle) , m_Data(rendererdata) {};
		RendererDataEntity(const RendererDataEntity& other) = default;
	public:
		//所有类型的组件均以AQRef的形式传入，ComponentType为组件类型（ComponentType不是Ref类型）
		template<typename ComponentType, typename... Args>	 AQRef<ComponentType> AddComponent(Args&&... args);
		template<typename ComponentType>	void RemoveComponent();
		template<typename ComponentType>	AQRef<ComponentType> GetComponent();
		template<typename ComponentType>	bool HasComponent();
	public:
		operator bool()const { return m_EntityHandle != entt::null; }
		operator AQUINT() const { return (AQUINT)m_EntityHandle; }
	private:
		entt::entity m_EntityHandle{ entt::null };
		entt::registry* m_Data=nullptr;
	};



	template<typename ComponentType, typename ...Args>
	AQRef<ComponentType> RendererDataEntity::AddComponent(Args&&... args)
	{
		return m_Data->emplace<AQRef<ComponentType>>(m_EntityHandle, ComponentType::Create(std::forward<Args>(args)...));	
	}

	template<typename ComponentType>
	void RendererDataEntity::RemoveComponent()
	{
	 if (!HasComponent<ComponentType>())
		{
			AQ_CORE_ASSERT(false, "RendererDataEntity::GetComponent:Failed to getcomponent, this entiy does not has this type of component!(entityhandle:{0})", (AQUINT)m_EntityHandle);
			AQ_CORE_ERROR("RendererDataEntity::GetComponent:Failed to getcomponent, this entiy does not has this type of component!(entityhandle:{0})", (AQUINT)m_EntityHandle);
			return;
		}
		else
		 m_Data->remove<AQRef<ComponentType>>(m_EntityHandle);
	}

	template<typename ComponentType>
	AQRef<ComponentType> RendererDataEntity::GetComponent()
	{
		if (!HasComponent<ComponentType>())
		{
			AQ_CORE_ASSERT(false, "RendererDataEntity::GetComponent:Failed to getcomponent, this entiy does not has this type of component!(entityhandle:{0})", (AQUINT)m_EntityHandle);
			AQ_CORE_ERROR("RendererDataEntity::GetComponent:Failed to getcomponent, this entiy does not has this type of component!(entityhandle:{0})", (AQUINT)m_EntityHandle);
			return AQRef<ComponentType>();
		}
		else
			return m_Data->get<AQRef<ComponentType>>(m_EntityHandle);
	}

	template<typename ComponentType>
	bool RendererDataEntity::HasComponent()
	{
		return m_Data->any_of<AQRef<ComponentType>>(m_EntityHandle);
	}
}



