#pragma once
#include "core/AquariusCore.h"
#include "core/AQObject.h"
#include "core/TimeSystem.h"
#include "entt/entt.hpp"




namespace Aquarius
{
	class AQEntity;

	class AQUARIUS_API AQScene: public AQObject
	{
	public:
		static AQRef<AQScene> Create();
		static AQRef<AQScene> Create(const std::string name);
	public:
		AQEntity CreateEntity(std::string name="");


		void OnUpdate(DeltaTime& dt);
		void OnRender(DeltaTime& dt);
		void OnViewportResize(AQUINT width, AQUINT height);
	protected:
		AQScene();
		AQScene(const std::string& name);
		~AQScene();
	public:
		entt::registry m_Registry;
		AQUINT m_ViewportWidth = 0;
		AQUINT m_ViewportHeight = 0;
	};

}