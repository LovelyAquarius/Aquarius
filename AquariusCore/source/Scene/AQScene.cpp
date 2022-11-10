#include "AQPCH.h"
#include "Renderer/Renderer2D.h"
#include "AQScene.h"
#include "AQEntity.h"

#include "ElementSystem/AQComponents/AQTransformComponent.h"
#include "ElementSystem/AQComponents/AQTagComponent.h"
#include "ElementSystem/AQComponents/AQColorComponent.h"
#include "ElementSystem/AQComponents/AQCameraComponent.h"
#include "ElementSystem/AQComponents/AQNativeScriptComponent.h"

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
		Eigen::Matrix4f* cameratransform = nullptr;
		//寻找场景主相机
		{
			auto view = m_Registry.view<AQRef<AQTransformComponent>, AQRef<AQCameraComponent>>();
			for (auto entity : view)
			{
				const auto& [transform, camera] = view.get<AQRef<AQTransformComponent>, AQRef<AQCameraComponent>>(entity);
				if (camera->IsPrimary)
				{
					primarycamera = &camera->Camera;
					cameratransform = &transform->Transform;
					break;
				}
			}
		}
		//___________________________________________________________
		//渲染
		if (primarycamera)
		{
			Renderer2D::BeginScene(*primarycamera, *cameratransform);
			
			auto group = m_Registry.group<AQRef<AQTransformComponent>>(entt::get<AQRef<AQColorComponent>>);
			for (auto entity : group)
			{
				const auto& [transform, color] = group.get< AQRef<AQTransformComponent>, AQRef<AQColorComponent> >(entity);
				Renderer2D::DrawQuad(transform->Transform, color->Color);
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

		entity.AddComponent<AQTransformComponent>(Eigen::Matrix4f().setIdentity());
		entity.AddComponent<AQTagComponent>(name);

		return entity;
	}


}