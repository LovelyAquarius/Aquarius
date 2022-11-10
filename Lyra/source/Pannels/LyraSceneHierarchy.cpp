#include "Aquarius.h"
#include "LyraSceneHierarchy.h"
#include "MathSystem/AQEigen.h"

#include "Scene/ElementSystem/AQComponents/AQTransformComponent.h"
#include "Scene/ElementSystem/AQComponents/AQTagComponent.h"
#include "Scene/ElementSystem/AQComponents/AQColorComponent.h"
#include "Scene/ElementSystem/AQComponents/AQCameraComponent.h"
#include "Scene/ElementSystem/AQComponents/AQNativeScriptComponent.h"


namespace Aquarius
{

	LyraSceneHierarchy::LyraSceneHierarchy(const AQRef<AQScene>& scene)
		:Layer("LyraSceneHierarchy")
	{
		SetContext(scene);
	}
	void LyraSceneHierarchy::SetContext(const AQRef<AQScene>& scene)
	{
		m_Scene = scene;
	}




	void LyraSceneHierarchy::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		m_Scene->m_Registry.each(
			[&](auto entityid)
			{
				AQEntity entity = AQEntity(entityid, m_Scene);
				DrawEntityNode(entity);
			});
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_Selection = {};
		ImGui::End();

		ImGui::Begin("Propertities");
		if (m_Selection)
			DrawProperties(m_Selection);

		ImGui::End();
	}

	void LyraSceneHierarchy::DrawEntityNode(AQEntity entity)
	{
		auto& tag = entity.GetComponent<AQTagComponent>()->Tag;

		void* ptr_id = (void*)(AQUINT64)(AQUINT)entity;

		ImGuiTreeNodeFlags flags = ((m_Selection == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow| ImGuiTreeNodeFlags_SpanFullWidth;

		bool isopened = ImGui::TreeNodeEx(ptr_id, flags, tag.c_str());
		if (ImGui::IsItemClicked())
			m_Selection = entity;
		if (isopened)
			ImGui::TreePop();
	}

	void LyraSceneHierarchy::DrawProperties(AQEntity entity)
	{
		if (entity.HasComponent<AQTagComponent>())
		{
			auto& tag = entity.GetComponent<AQTagComponent>()->Tag;

			static AQCHAR buffer[256];
			memset(buffer, 0, AQ_CORE_C_ARRAYSIZE(buffer));
			strcpy(buffer, tag.c_str());

			if (ImGui::TreeNodeEx((void*)AQ_CORE_TYPE_HASHCODE(AQTagComponent),  ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth, "Tag"))
			{
				if(ImGui::InputText("##Tag", buffer, AQ_CORE_C_ARRAYSIZE(buffer)))
					tag = std::string(buffer);
				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<AQTransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)AQ_CORE_TYPE_HASHCODE(AQTransformComponent), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth, "Transform"))
			{
				auto& transform = entity.GetComponent<AQTransformComponent>()->Transform;
				ImGui::DragFloat3("##Transform", transform.col(3).data(), 0.1f);
				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<AQCameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)AQ_CORE_TYPE_HASHCODE(AQCameraComponent), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth, "Camera"))
			{
				auto& camera = entity.GetComponent<AQCameraComponent>()->Camera;

				static const char* str_orthographic = "Orthographic";
				static const char* str_perspective = "Perspective";
				static const char* projectionstrings[] = { str_orthographic ,str_perspective };
				const char* currentprojectionstring = projectionstrings[(int)camera.GetProjectionType()];
				ImGui::Text("Camera Projection");
				ImGui::Text("%.3f,  %.3f,  %.3f,  %.3f", camera.GetProjection().col(0).x(), camera.GetProjection().col(1).x(), camera.GetProjection().col(2).x(), camera.GetProjection().col(3).x());
				ImGui::Text("%.3f,  %.3f,  %.3f,  %.3f", camera.GetProjection().col(0).y(), camera.GetProjection().col(1).y(), camera.GetProjection().col(2).y(), camera.GetProjection().col(3).y());
				ImGui::Text("%.3f,  %.3f,  %.3f,  %.3f", camera.GetProjection().col(0).z(), camera.GetProjection().col(1).z(), camera.GetProjection().col(2).z(), camera.GetProjection().col(3).z());
				ImGui::Text("%.3f,  %.3f,  %.3f,  %.3f", camera.GetProjection().col(0).w(), camera.GetProjection().col(1).w(), camera.GetProjection().col(2).w(), camera.GetProjection().col(3).w());
				if (ImGui::BeginCombo("##Projection", currentprojectionstring))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isselected = currentprojectionstring == projectionstrings[i];
						if (ImGui::Selectable(projectionstrings[i], isselected))
						{
							currentprojectionstring = projectionstrings[i];
							camera.SetProjectionType((AQSceneCamera::ProjectionType)i);
							camera.SetPerspectiveFov(camera.GetPerspectiveFov());
						}
						if (isselected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == AQSceneCamera::ProjectionType::Orthographic)
				{
					float orthosize = camera.GetOrthoGraphicSize();
					AQ_CORE_IMGUI_LEFT_LABEL_IFSTRUCTURE("Size", ImGui::DragFloat("", &orthosize),
						{camera.SetOrthoGraphicSize(orthosize);});

					float orthonear_far[2] = { camera.GetOrthoGraphicNear() ,camera.GetOrthoGraphicFar() };
	
					AQ_CORE_IMGUI_LEFT_LABEL_IFSTRUCTURE("Near And Far", ImGui::DragFloat2("##Near And Far", orthonear_far),
						{
							camera.SetOrthoGraphicNear(orthonear_far[0]);
							camera.SetOrthoGraphicFar(orthonear_far[1]);
						});


				}
				if (camera.GetProjectionType() == AQSceneCamera::ProjectionType::Perspective)
				{
					float perspectivefov = glm::degrees(camera.GetPerspectiveFov());
					AQ_CORE_IMGUI_LEFT_LABEL_IFSTRUCTURE("Vertical Fov ", ImGui::DragFloat("", &perspectivefov),
						{ camera.SetPerspectiveFov(glm::radians(perspectivefov)); });

					float perspectivenear_far[2] = { camera.GetPerspectiveNear() ,camera.GetPerspectiveFar() };

					AQ_CORE_IMGUI_LEFT_LABEL_IFSTRUCTURE("Near And Far", ImGui::DragFloat2("##Near And Far", perspectivenear_far),
						{
							camera.SetPerspectiveNear(perspectivenear_far[0]);
							camera.SetPerspectiveFar(perspectivenear_far[1]);
						});
				}


			}

		}

	}


}