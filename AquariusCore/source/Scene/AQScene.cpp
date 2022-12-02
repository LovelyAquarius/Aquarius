#include "AQPCH.h"
#include "Renderer/Renderer2D/Renderer2D.h"
#include "AQScene.h"
#include "AQEntity.h"

#include "ElementSystem/AQComponents/AQComponentHeader.h"


namespace Aquarius
{

	

	AQRef<AQScene> AQScene::Create()
	{
		return new AQScene();
	}

	AQRef<AQScene> AQScene::Create(const std::string name)
	{
		return new AQScene(name);
	}

	void AQScene::OnRenderEditor(DeltaTime& dt, EditorCamera& camera)
	{
		if (camera.GetViewportSize() != Renderer2D::GetRenderViewPort())
			RenderCommand::SetViewport(0,0,camera.GetViewportSize().x(), camera.GetViewportSize().y());

		Renderer2D::BeginScene(camera);
		auto group = m_Registry.group<AQRef<AQTransformComponent>>(entt::get<AQRef<AQSpiriteComponent>>);
		for (auto entity : group)
		{
			const auto& [transform, spirite] = group.get< AQRef<AQTransformComponent>, AQRef<AQSpiriteComponent> >(entity);
			Renderer2D::DrawSpirite(transform->GetTransform(), spirite, (AQUINT)entity);
		}
		Renderer2D::EndScene();

	}

	void AQScene::OnUpdate(DeltaTime& dt)
	{
		//更新Script
		{
			m_Registry.view<AQRef<AQNativeScriptComponent>>().each(
				[=](auto entity, auto& nsc)//nsc:nativescriptablecomponent
				{
					if (!nsc->Instance)
					{
						nsc->Instance = nsc->CreateScript();
						AQRef<AQScene> scene = AQRef<AQScene>(this);
						nsc->Instance->m_Entity = AQEntity(entity, scene);
						nsc->Instance->OnCreate();
					}
					nsc->Instance->OnUpdate(dt);
				});
		}




	}

	void AQScene::OnRender(DeltaTime& dt)
	{

		AQCamera* primarycamera = nullptr;
		Eigen::Matrix4f cameratransform;
		//寻找场景主相机
		{
			auto view = m_Registry.view<AQRef<AQTransformComponent>, AQRef<AQCameraComponent>>();
			for (auto& entity : view)
			{
				const auto& [transform, camera] = view.get<AQRef<AQTransformComponent>, AQRef<AQCameraComponent>>(entity);
				if (camera->IsPrimary)
				{
					primarycamera = &camera->Camera;
					cameratransform = transform->GetTransform();
					break;
				}
			}
		}
		//___________________________________________________________
		//渲染
		if (primarycamera)
		{
			Renderer2D::BeginScene(*primarycamera, cameratransform);

			auto group = m_Registry.group<AQRef<AQTransformComponent>>(entt::get<AQRef<AQSpiriteComponent>>);
			for (auto entity : group)
			{
				const auto& [transform, color] = group.get< AQRef<AQTransformComponent>, AQRef<AQSpiriteComponent> >(entity);
				Renderer2D::DrawQuad(transform->GetTransform(), color->Color);
			}

			Renderer2D::EndScene();
		}
		//__________________________________________________-
	}



	void AQScene::OnViewportResize(AQUINT width, AQUINT height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<AQRef<AQCameraComponent>>();
		for (auto entity : view)
		{
			auto& cameracomponent = view.get<AQRef<AQCameraComponent>>(entity);
			if (!cameracomponent->FixedAspectRatio)
				cameracomponent->Camera.SetViewportSize((AQFLOAT)width, (AQFLOAT)height);
		}
	}

	AQScene::AQScene()
	{
		AQ_INITIALIZE_AQOBJECT_TYPE(AQScene);
	}
	AQScene::AQScene(const std::string& name)
	{
		AQ_INITIALIZE_AQOBJECT_NAME_AND_TYPE(name, AQScene);
	}
	AQScene::~AQScene()
	{

	}

	AQEntity AQScene::CreateEntity(std::string name)
	{
		entt::entity entt = m_Registry.create();
		AQRef<AQScene> scene = AQRef<AQScene>(this);
		AQEntity entity = AQEntity(entt, scene);

		entity.AddComponent<AQTransformComponent>();
		entity.AddComponent<AQTagComponent>(name);

		return entity;
	}

	void AQScene::DestroyEntity(AQEntity& entity)
	{
		m_Registry.destroy(entity);
		entity.m_EntityHandle = entt::null;
		entity.m_Scene = nullptr;
	}
	AQEntity AQScene::GetPrimaryCamera()
	{
		auto view = m_Registry.view<AQRef<AQCameraComponent>>();
		for (auto entity : view)
		{
			const auto cameracomponent = view.get<AQRef<AQCameraComponent>>(entity);
			if (cameracomponent->IsPrimary)
			{
				AQRef<AQScene > scene{ this };
				return AQEntity{ entity , scene };
			}	
		}
		return {};
	}
}
namespace Aquarius
{
	template<typename ComponentType>
	void AQScene::OnComponentAdded(AQEntity entity, AQRef<ComponentType>& component)
	{
		AQ_ASSERT(false,"AQScene::OnComponentAdded: Failed! Check to see AQScene.cpp!")
	}

	template<>
	void AQScene::OnComponentAdded<AQTagComponent>(AQEntity entity, AQRef<AQTagComponent>& component)
	{}
	template<>
	void AQScene::OnComponentAdded<AQTransformComponent>(AQEntity entity, AQRef<AQTransformComponent>& component)
	{}
	template<>
	void AQScene::OnComponentAdded<AQCameraComponent>(AQEntity entity, AQRef<AQCameraComponent>& component)
	{
		component->Camera.SetViewportSize((float)m_ViewportWidth, (float)m_ViewportHeight);
	}
	template<>
	void AQScene::OnComponentAdded<AQSpiriteComponent>(AQEntity entity, AQRef<AQSpiriteComponent>& component)
	{}
	template<>
	void AQScene::OnComponentAdded<AQNativeScriptComponent>(AQEntity entity, AQRef<AQNativeScriptComponent>& component)
	{}
}