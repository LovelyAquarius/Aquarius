#pragma once
#include "core/AquariusCore.h"
#include "RendererDataEntity.h"
#include "Renderer2DComponents/CommonTexture2DData.h"
#include "Renderer2DComponents/QuadVertex2DData.h"
#include "Renderer2DComponents/CommonCamera2DData.h"
#include<entt/entt.hpp>

namespace Aquarius
{
	

	class AQUARIUS_API Renderer2DData
	{
	public:
		struct  Statistics
		{
			AQUINT DrawCalls = 0;

			AQUINT QuadCount = 0;
			AQUINT LineVertexCount = 0;
			AQUINT QuadraticBezierUnitCount = 0;

			AQUINT GetQuadVertexCount() { return QuadCount * 4; }
			AQUINT GetQuadElementCount() { return QuadCount * 6; }
			AQUINT GetBezierVertexCount() { return LineVertexCount; }
			AQUINT GetQudraticBezierUnitCount() { return QuadraticBezierUnitCount; }
		};
	public:
		static Renderer2DData* CreateData() {return new Renderer2DData();}
	public:

		template<typename DataComponent, typename... Args>
		RendererDataEntity CreateEntity(Args&&... args);
		template<typename DataComponent>
		void RemoveEntity();
		template<typename DataComponent>
		AQRef<DataComponent> GetEntityComponent();
		template<typename DataComponent>
		AQBOOL HasEntity();
	private:
		Renderer2DData();
	public:
		Statistics m_Statistics;
		entt::registry m_Registry;
		std::vector<RendererDataEntity> m_Subdatas;
		static Renderer2DData* s_Renderer2DData;
	};


	template<typename DataComponent, typename... Args>
	RendererDataEntity Renderer2DData::CreateEntity(Args&&... args)
	{
		for (auto& entity : m_Subdatas)
		{
			if (entity.HasComponent<DataComponent>())
			{
				AQ_CORE_WARN("Renderer2DData::CreateEntity: The DataComponent has already been created!");
				return RendererDataEntity();
			}
		}

		entt::entity entt = m_Registry.create();
		RendererDataEntity entity = RendererDataEntity(entt, &m_Registry);
		entity.AddComponent<DataComponent>(std::forward<Args>(args)...);
		m_Subdatas.emplace_back(entity);
		return entity;
	}

	template<typename DataComponent>
	void Renderer2DData::RemoveEntity()
	{
		static_assert(!std::is_same_v<DataComponent, CommonTexture2DData>, "Renderer2DData::RemoveEntity:CommonTexture2DData is not allowed to be removed!");
		static_assert(!std::is_same_v<DataComponent, QuadVertex2DData>, "Renderer2DData::RemoveEntity:QuadVertex2DData is not allowed to be removed!");
		static_assert(!std::is_same_v<DataComponent, CommonCamera2DData>, "Renderer2DData::RemoveEntity:CommonCamera2DData is not allowed to be removed!");

		for (auto& entity : m_Subdatas)
		{
			if (entity.HasComponent<DataComponent>())
			{
				entity.RemoveComponent<DataComponent>();
				m_Subdatas.erase(entity);
				return;
			}
		}
	}

	template<typename DataComponent>
	AQRef<DataComponent> Renderer2DData::GetEntityComponent()
	{
		for (auto& entity : m_Subdatas)
		{
			if (entity.HasComponent<DataComponent>())
			{
				return entity.GetComponent<DataComponent>();
			}
		}
		return nullptr;
	}

	template<typename DataComponent>
	AQBOOL Renderer2DData::HasEntity()
	{
		for (auto& entity : m_Subdatas)
		{
			if (entity.HasComponent<DataComponent>())
			{
				return true;
			}
		}
		return false;
	}

}


