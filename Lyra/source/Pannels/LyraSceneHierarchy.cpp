#include "LyraPCH.h"
#include "Aquarius.h"
#include "LyraSceneHierarchy.h"
#include "MathSystem/AQMathEx.h"
#include"Scene/ElementSystem/AQComponents/AQComponentHeader.h"
#include "UISystem/IMGUILayer/AQImguiLibrary.h"
#include <IconsFontAwesome/IconsFontAwesome6.h>
#include<IMGUI/imgui_internal.h>

namespace Aquarius
{
	static AQBOOL DrawVec3Control(const std::string& label, Eigen::Vector3f& values, AQFLOAT dragstep=0.01f, AQFLOAT resetvalue = 0.0f, AQFLOAT columnwidth = 120.0f);
	template<typename Componenttype, typename UIFunction>
	static void DrawControllableComponent(const std::string& label, AQEntity entity, UIFunction function);
	
}

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
		m_Selection = {};
	}

	void LyraSceneHierarchy::SetSelectedEntity(AQEntity& entity)
	{
		m_Selection = entity;
	}




	void LyraSceneHierarchy::OnImGuiRender()
	{
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

			if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
			{
				if (ImGui::MenuItem("Create Empty Entity"))
					m_Scene->CreateEntity("Unnamed Entity");
			}

			ImGui::End();
		}


		ImGui::Begin("Propertities");
		if (m_Selection)
		{
			DrawProperties(m_Selection);
		}
			

		ImGui::End();
	}

	void LyraSceneHierarchy::DrawEntityNode(AQEntity entity)
	{
		static const ImGuiTreeNodeFlags commonflgas = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		auto& tag = entity.GetComponent<AQTagComponent>()->Tag;
		void* ptr_id = (void*)(AQUINT64)(AQUINT)entity;

		ImGuiTreeNodeFlags flags = ((m_Selection == entity) ? ImGuiTreeNodeFlags_Selected : 0) | commonflgas;

		bool isopened = AQImgui_TreeNodeEx(ptr_id, flags, tag.c_str());
		if (ImGui::IsItemClicked())
			m_Selection = entity;

		bool IsAlive = true;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				IsAlive=false;
			ImGui::EndPopup();
		}


		if (isopened)
			ImGui::TreePop();

		if (!IsAlive)
		{
			m_Scene->DestroyEntity(entity);
			if (m_Selection == entity)
				m_Selection = {};
		}
			
	}
	
	void LyraSceneHierarchy::DrawProperties(AQEntity entity)
	{
		static const ImGuiTreeNodeFlags common_treenodeflags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_SpanAvailWidth;


		if (entity.HasComponent<AQTagComponent>())
		{
			auto& tag = entity.GetComponent<AQTagComponent>()->Tag;

			static char buffer[256];
			memset(buffer, 0, AQ_CORE_C_ARRAYSIZE(buffer));
			strcpy(buffer, tag.c_str());

			bool open = ImGui::TreeNodeEx((void*)AQ_CORE_TYPE_HASHCODE(AQTagComponent), common_treenodeflags, "Tag");

			if (open)
			{
				if(ImGui::InputText("##Tag", buffer, AQ_CORE_C_ARRAYSIZE(buffer)))
					tag = std::string(buffer);
				ImGui::TreePop();
			}

		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");
		if (ImGui::BeginPopup("AddComponent"))
		{
			if (ImGui::MenuItem("CameraComponent"))
			{
				m_Selection.AddComponent<AQCameraComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("ColorComponent"))
			{
				m_Selection.AddComponent<AQSpiriteComponent>();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();
		        

		DrawControllableComponent<AQTransformComponent>("Transform", entity, [](auto& component)
		{
			DrawVec3Control("Translation", component->Translation);
			static bool degreemode = true;
			if (degreemode)
			{
				Eigen::Vector3f rotationdegree;
				rotationdegree << (AQFLOAT)AQ_RadianToDegree(component->Rotation.x()), (AQFLOAT)AQ_RadianToDegree(component->Rotation.y()), (AQFLOAT)AQ_RadianToDegree(component->Rotation.z());
				if (DrawVec3Control("Rotation", rotationdegree, 1.0f))
					component->Rotation << AQ_DegreeToRadian(rotationdegree.x()), AQ_DegreeToRadian(rotationdegree.y()), AQ_DegreeToRadian(rotationdegree.z());
			}
			else
				DrawVec3Control("Rotation", component->Rotation, 0.01f);

			DrawVec3Control("Scale", component->Scale, 0.1f, 1.0f);
		});

		DrawControllableComponent<AQCameraComponent>("Camera", entity, [](auto& component) 
		{
			auto& camera = component->Camera;
			static const char* str_orthographic = "Orthographic";
			static const char* str_perspective = "Perspective";
			static const char* projectionstrings[] = { str_orthographic ,str_perspective };
			const char* currentprojectionstring = projectionstrings[(int)camera.GetProjectionType()];
			ImGui::Text("Camera Projection");
			ImGui::Text("%.3f,  %.3f,  %.3f,  %.3f", camera.GetProjection().col(0).x(), camera.GetProjection().col(1).x(), camera.GetProjection().col(2).x(), camera.GetProjection().col(3).x());
			ImGui::Text("%.3f,  %.3f,  %.3f,  %.3f", camera.GetProjection().col(0).y(), camera.GetProjection().col(1).y(), camera.GetProjection().col(2).y(), camera.GetProjection().col(3).y());
			ImGui::Text("%.3f,  %.3f,  %.3f,  %.3f", camera.GetProjection().col(0).z(), camera.GetProjection().col(1).z(), camera.GetProjection().col(2).z(), camera.GetProjection().col(3).z());
			ImGui::Text("%.3f,  %.3f,  %.3f,  %.3f", camera.GetProjection().col(0).w(), camera.GetProjection().col(1).w(), camera.GetProjection().col(2).w(), camera.GetProjection().col(3).w());
			AQ_CORE_IMGUI_LEFT_LABEL_IFSTRUCTURE("Is Primary", ImGui::Checkbox("##Is Primary", &component->IsPrimary), {});

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
				float orthosize = camera.GetOrthographicSize();
				AQ_CORE_IMGUI_LEFT_LABEL_IFSTRUCTURE("ViewSize", ImGui::DragFloat("##ViewSize", &orthosize),
					{ camera.SetOrthographicSize(orthosize); });

				float orthonear_far[2] = { camera.GetOrthographicNear() ,camera.GetOrthographicFar() };

				AQ_CORE_IMGUI_LEFT_LABEL_IFSTRUCTURE("Near And Far", ImGui::DragFloat2("##Near And Far", orthonear_far),
					{
						camera.SetOrthographicNear(orthonear_far[0]);
						camera.SetOrthographicFar(orthonear_far[1]);
					});


			}
			if (camera.GetProjectionType() == AQSceneCamera::ProjectionType::Perspective)
			{
				float perspectivefov = glm::degrees(camera.GetPerspectiveFov());
				AQ_CORE_IMGUI_LEFT_LABEL_IFSTRUCTURE("Vertical Fov ", ImGui::DragFloat("##Vertical Fov ", &perspectivefov),
					{ camera.SetPerspectiveFov(AQ_DegreeToRadian(perspectivefov)); });

				float perspectivenear_far[2] = { camera.GetPerspectiveNear() ,camera.GetPerspectiveFar() };

				AQ_CORE_IMGUI_LEFT_LABEL_IFSTRUCTURE("Near And Far", ImGui::DragFloat2("##Near And Far", perspectivenear_far),
					{
						camera.SetPerspectiveNear(perspectivenear_far[0]);
						camera.SetPerspectiveFar(perspectivenear_far[1]);
					});
			}
		});


		DrawControllableComponent<AQSpiriteComponent>("Color", entity, [](auto& component)
		{
			AQ_CORE_IMGUI_LEFT_LABEL_IFSTRUCTURE("Color RGBA", ImGui::ColorEdit4("##ColorComponent", component->Color.data()), {});
		});
	}
}

namespace Aquarius
{
	AQBOOL DrawVec3Control(const std::string& label, Eigen::Vector3f& values, AQFLOAT dragstep, AQFLOAT resetvalue , AQFLOAT columnwidth)
	{
		bool datachanged = false;
		ImGui::PushID(label.c_str());

		ImGui::BeginColumns("##column", 2, ImGuiColumnsFlags_NoResize);
		ImGui::SetColumnWidth(0, columnwidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });
		float lineheight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonsize = { lineheight + 3.0f,lineheight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f,0.1f,0.15f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f,0.2f,0.2f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f,0.1f,0.15f,1.0f });
		if (ImGui::Button("X", buttonsize))
		{
			values.x() = resetvalue;
			datachanged = true;
		}
		ImGui::SameLine();
		if (ImGui::DragFloat("##X", &values.x(), dragstep))
			datachanged = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f,0.7f,0.3f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f,0.8f,0.3f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f,0.7f,0.2f,1.0f });
		if (ImGui::Button("Y", buttonsize))
		{
			values.y() = resetvalue;
			datachanged = true;
		}
			
		ImGui::SameLine();
		if(ImGui::DragFloat("##Y", &values.y(), dragstep))
			datachanged = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f,0.25f,0.8f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f,0.35f,0.9f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f,0.25f,0.8f,1.0f });
		if (ImGui::Button("Z", buttonsize))
		{
			values.z() = resetvalue;
			datachanged = true;
		}
			
		ImGui::SameLine();
		if(ImGui::DragFloat("##Z", &values.z(), dragstep))
			datachanged = true;
		ImGui::PopItemWidth();
		ImGui::PopStyleColor(3);

	    ImGui::PopStyleVar();
		ImGui::EndColumns();

		ImGui::Columns(1);

		ImGui::PopID();
		return datachanged;
	}
	
	template<typename Componenttype, typename UIFunction>
	void DrawControllableComponent(const std::string& label, AQEntity entity, UIFunction function)
	{
		if (entity.HasComponent<Componenttype>())
		{
			static const ImGuiTreeNodeFlags common_treenodeflags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed ;
			bool is_ComponentAlive = true;
			auto component = entity.GetComponent<Componenttype>();
			ImVec2 contentregionavailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4,4 });
			float lineheight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)AQ_CORE_TYPE_HASHCODE(Componenttype), common_treenodeflags, label.c_str());
			ImGui::PopStyleVar();
			
			ImGui::SameLine(contentregionavailable.x - lineheight * 1.0f);

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f,0.0f,0.0f,0.0f });
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.313f,0.450f,0.862f,1.0f });
			bool button =ImGui::Button(ICON_FA_SUN, ImVec2{ lineheight, lineheight });
			ImGui::PopStyleColor(2);

			if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
			{
				static const char* settingbuttontip = "组件设置";
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 8.0f,8.0f });
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::Text(settingbuttontip);
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
				ImGui::PopStyleVar();
			}
			if (button)
			{
				ImGui::OpenPopup("ComponentSettings");
			}


			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
					is_ComponentAlive = false;
				ImGui::EndPopup();
			}

			if (open)
			{
				function(component);
				ImGui::TreePop();
			}

			if (!is_ComponentAlive)
				entity.RemoveComponent<Componenttype>();
		}

	}




}