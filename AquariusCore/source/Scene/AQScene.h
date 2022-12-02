#pragma once
#include "core/AquariusCore.h"
#include "core/AQObject.h"
#include "Renderer/EditorCamera.h"
#include "Renderer/AQFrameBuffer.h"
#include "core/TimeSystem.h"
#include<entt/entt.hpp>



namespace Aquarius
{
	class AQEntity;

	class AQUARIUS_API AQScene: public AQObject
	{
		friend class AQEntity;
		friend class LyraSceneHierarchy;
		friend class SceneSerializer;
	public:
		enum RenderMode
		{
			Default=0,
			FrameBuffer=1,
		};
	public:
		static AQRef<AQScene> Create();
		static AQRef<AQScene> Create(const std::string name);
	public:
		AQEntity CreateEntity(std::string name="");
		void DestroyEntity(AQEntity& entity);
		AQEntity GetPrimaryCamera();

	
		void OnUpdate(DeltaTime& dt);
		void OnRender(DeltaTime& dt);
		void OnRenderEditor(DeltaTime& dt, EditorCamera& camera);
		void OnRenderEditor(AQRef<AQFrameBuffer>& fbo,DeltaTime& dt, EditorCamera& camera);
		void OnViewportResize(AQUINT width, AQUINT height);
	private:
		template<typename ComponentType>
		void OnComponentAdded(AQEntity entity, AQRef<ComponentType>& component);

	protected:
		AQScene();
		AQScene(const std::string& name);
		~AQScene();
	private:
		entt::registry m_Registry;
		AQUINT m_ViewportWidth = 1600;
		AQUINT m_ViewportHeight = 900;
	};

}