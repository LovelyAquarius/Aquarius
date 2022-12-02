#pragma once
#include "Aquarius.h"
#include "Pannels/LyraSceneHierarchy.h"
#include "Pannels/LyraContentBrowser.h"
#include "Renderer/EditorCamera.h"
#include "Utils/AQFont/AQTextImage.h"

namespace Aquarius
{

	class  LyraEditor :public Layer
	{
	public:

	public:
		LyraEditor();
		~LyraEditor();
		virtual void OnAttach()override;
		virtual void OnUpdate(DeltaTime& dt)override;
		virtual void	 OnRender(DeltaTime& dt)override;
		virtual void OnEvent(BaseEvent& event)override;
		virtual void  OnImGuiRender()override;
		void PushLayer(Layer* layer);
		void PushOverLay(Layer* layer);

	private:
		void LyraEditor_ImGui_Style_Black(ImGuiStyle* dst=NULL);
		void ImguiDocking(bool* p_open);
		//独立事件
		bool OnKeyPressed(KeyPressedEvent& event);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);
		void Openfile();
		void Savefileas();
	private:
		//主要附件
		static Translation s_Translation;
		LayerStack m_LayerStack;
		LyraSceneHierarchy* m_SceneHierarchy;
		LyraContentBrowser* m_ContentBrowser;
		EditorCamera m_EditorCamera;
		EditorCamera m_StaticCamera;
		AQINT m_GuizmoType = 7;
		AQShaderLibrary m_Shaderlib;
		AQParticcle2D m_Particle2D;
		AQParticcle2D::ParticleProperties m_ParticleSetting;

		//___________________________________
		// 视窗属性
		AQRef<AQFrameBuffer> m_FrameBuffer;
		AQBOOL m_ViewportFocused = false;
		AQBOOL m_ViewportHovered = false;
		Eigen::Vector2f m_ViewportSize;
		Eigen::Vector2f m_ViewportBounds[2];
		AQEntity m_HoveredEntity;
		// __________________________________
		//camera相关属性
		OrthgraphicCameraController m_CameraController;
		//_______________________
		// 场景
		AQRef<AQScene> m_BackScene;//总背景
		AQEntity m_SquareEntity;
		AQEntity m_BackCamera;
		AQRef<AQScene> m_ActiveScene;//视窗场景
		// ————————————————————————————————————
		//物体相关属性
		glm::vec3 m_Object_1_position;
		float m_Object_1_Rotation = 0.0f;
		float m_Object_1_MovingSpeed = 1.0f;
		float m_Object_1_Rotationspeed = 0.0f;

		glm::vec3 m_Object_2_scale{ 0.1f,0.1f,0.0f };
		glm::vec4 m_Object_2_color{ 0.2f,0.3f,0.8f ,1.0f };
	};




	





}
