#include "LyraEditor.h"
#include "core/APPGUI.h"
#include "MathSystem/AQEigen.h"


#include "Scene/ElementSystem/AQComponents/AQTransformComponent.h"
#include "Scene/ElementSystem/AQComponents/AQTagComponent.h"
#include "Scene/ElementSystem/AQComponents/AQColorComponent.h"
#include "Scene/ElementSystem/AQComponents/AQCameraComponent.h"
#include "Scene/ElementSystem/AQComponents/AQNativeScriptComponent.h"
#include "ScriptSystem/CoreScripts/AQCameraController.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <IMGUI/imgui_internal.h>

namespace Aquarius
{

	
	float onesecondcount = 0.0f;
	float fivesecondcount = 0.0f;
	int framecount = 0;
	glm::vec4 red(0.8f, 0.2f, 0.3f, 1.0f);
	glm::vec4 blue(0.2f, 0.3f, 0.8f, 1.0f);
	glm::vec3 randompotision;


	AQRef<AQTexture2D> backpicture;
	AQRef<AQTexture2D> bilibilipicture;
	AQRef<AQTexture2D> mypicture;
	AQRef<AQSubTexture2D> spirite;




	AQRef<AQVertexArray>  testvao;
	AQRef<AQVertexBuffer>  testvbo;
	AQRef<AQElementBuffer> testebo;
	AQRef<AQShader> testshader;

	glm::vec2 center = { 0.0f,0.0f };
	float radius = 1.0f;
	float magicnumber = (4.0f / 3.0f) * glm::tan(2.0f * glm::pi<float>() / 2.0f / 4.0f);
	std::vector<glm::vec2> quadraticbezier;
	glm::vec2 direction;

	std::vector<AQRef<AQQuadraticBezierCurve2D>> bezier;
	char utf8[10] = "";
	char utf8_old[5] = "";
	char unicode[10];
	glm::mat4 pp;
	glm::vec4 mm;
	Eigen::Vector4f m;



	LyraEditor::LyraEditor() :Layer("LyraEditor"), m_CameraController(1280.0f / 720.0f), m_Object_1_position(0.0f, 0.0f, 0.0f), m_Particle2D(30000)
	{
		m_ViewportSize << 0.0f, 0.0f;

		m_ActiveScene = AQScene::Create("myscene");

		//设置子项的栈
		PushOverLay(new LyraSceneHierarchy(m_ActiveScene));
		

		const char* bilifile = R"(G:\Mine\CppProjects\Aquarius\AquariusCore\source\Data\bilibili.png)";
		const char* picf = R"(G:\Mine\CppProjects\Aquarius\AquariusCore\source\Data\Checkerboard.png)";
		const char* mypicfile = R"(G:\Mine\CppProjects\Aquarius\AquariusCore\source\Data\lovely.png)";
		backpicture = AQTexture2D::Create(picf);
		bilibilipicture = AQTexture2D::Create(bilifile);
		mypicture = AQTexture2D::Create(mypicfile);
		//_____________________________________-
		spirite = AQSubTexture2D::Create(bilibilipicture, { 162,124 }, { 1,1 });

	}

	void LyraEditor::OnAttach()
	{
		//ImGui初始化
		LyraEditor_ImGui_Style_Black();

		//framebuffer config
		AQFrameBufferConfiguration fbconfig;
		fbconfig.Width = 1280;
		fbconfig.Height = 720;
		m_FrameBuffer =AQFrameBuffer::Create(fbconfig);
		//entity config

		m_SquareEntity = m_ActiveScene->CreateEntity("square");
		m_SquareEntity.AddComponent<AQColorComponent>(Eigen::Vector4f().setConstant(1.0f));
		m_CameraEntity = m_ActiveScene->CreateEntity("maincamera");
		m_CameraEntity.AddComponent<AQCameraComponent>(AQ_Ortho(-16.0f,16.0f,-1.0f,1.0f,-9.0f,9.0f));
		m_CameraEntity.AddComponent<AQNativeScriptComponent>()->Bind<AQCameraController>();

	}
	void LyraEditor::OnUpdate(DeltaTime& dt)
	{
		AQ_TIME_MONITOR("LyraEditor::OnUpdate");
		using namespace Aquarius;
		//监视帧率
		if (onesecondcount < 1.0f)
		{
			onesecondcount += dt;
			framecount++;
		}
		else
		{
			AQ_CLIENT_TRACE("客户端帧率:{0}fps", framecount);
			onesecondcount = 0.0f;
			onesecondcount += dt;
			framecount = 1;
		}
		if (fivesecondcount < 3.0f)
		{
			fivesecondcount += dt;
		}
		else
		{
			float x = AQRandom::Float() * m_ViewportSize.x();
			float y = AQRandom::Float() * m_ViewportSize.y();
			float r = AQRandom::Float() * 40;
			auto width = Application::Get().GetWindow().GetWidth();
			auto height = Application::Get().GetWindow().GetHeight();

			auto bounds = m_CameraController.GetBounds();
			auto position = m_CameraController.GetCamera().GetPosition();
			x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
			y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
			m_ParticleSetting.Position = { x + position.x(), y + position.y()};
			for (int i = 0; i < r; i++)
				m_Particle2D.Emit(m_ParticleSetting);

			fivesecondcount = 0.0f;
			fivesecondcount += dt;
		}
		//____________________________________________
		//改变大小
		{
			AQFrameBufferConfiguration config = m_FrameBuffer->GetConfiguration();
			if (m_ViewportSize.x() > 0.0f && m_ViewportSize.y() > 0.0f && (config.Width != m_ViewportSize.x() || config.Height != m_ViewportSize.y()))
			{
				m_FrameBuffer->Resize((AQUINT)m_ViewportSize.x(), (AQUINT)m_ViewportSize.y());
				m_CameraController.OnResize(m_ViewportSize.x(), m_ViewportSize.y());
				m_CameraEntity.GetComponent<AQCameraComponent>()->Camera.SetViewportSize((AQUINT)m_ViewportSize.x(), (AQUINT)m_ViewportSize.y());
			}
		}
		//____________________________________________
		m_ActiveScene->OnUpdate(dt);
		// camera控制
		if(m_ViewportFocused)
			m_CameraController.OnUpdate(dt);
		//__________________________________
		 //object1控制
		if (Input::IsKeyPressed(AQ_KEY_J))
			m_Object_1_position.x -= m_Object_1_MovingSpeed * dt;
		else if (Input::IsKeyPressed(AQ_KEY_L))
			m_Object_1_position.x += m_Object_1_MovingSpeed * dt;

		if (Input::IsKeyPressed(AQ_KEY_K))
			m_Object_1_position.y -= m_Object_1_MovingSpeed * dt;
		else if (Input::IsKeyPressed(AQ_KEY_I))
			m_Object_1_position.y += m_Object_1_MovingSpeed * dt;
		//___________________________________
	   //particle控制
		m_Particle2D.OnUpdate(dt);
		//_____________________________________

	}

	void	 LyraEditor::OnRender(DeltaTime& dt)
	{
		AQ_TIME_MONITOR("LyraEditor::OnRender");
		//清除所有状态
		Renderer2D::ResetStatistics();
		//__________________________________________________
		//渲染camera
		{
			AQ_TIME_MONITOR("Renderer2D::Scene");
			m_FrameBuffer->Bind();
			//初始化渲染
			RenderCommand::SetClearcolor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RenderCommand::Clear();
			//__________________________________________________
			Renderer2D::BeginScene(m_CameraController.GetCamera());
			if (*(unsigned short*)utf8 == *(unsigned short*)utf8_old || *(unsigned short*)utf8 == 0)
			{
			}
			else
			{
				*(unsigned short*)utf8_old = *(unsigned short*)utf8;
				AQ_UTF8ToUnicode(utf8, unicode);
				AQFontAPI::GetAQFontAPI()->GetSinglecharacterBezier(*(unsigned short*)unicode, bezier, 0.001f);
			}

			//for (int i = 0; i < bezier.size(); i++)
			//{
			//	Renderer2D::DrawBezier_Line_GPU(bezier[i], 20, red, 5.0f);
			//}

			Renderer2D::EndScene();

			m_ActiveScene->OnRender(dt);
			m_FrameBuffer->UnBind();
		}
		{
			RenderCommand::SetClearcolor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RenderCommand::Clear();
			Renderer2D::BeginScene(m_CameraController.GetCamera());
			Renderer2D::EndScene();
		}
	}


	void LyraEditor::OnEvent(BaseEvent& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(AQ_BIND_EVENT_FN(LyraEditor::OnKeyPressed));
		m_CameraController.OnEvent(event);
	}




	void LyraEditor::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void LyraEditor::PushOverLay(Layer* layer)
	{
		m_LayerStack.PushOverLay(layer);
		layer->OnAttach();
	}












	bool open = true;
	void  LyraEditor::OnImGuiRender()
	{
		//bool imguidockingenable = true;
		//ImguiDocking(&imguidockingenable);

		ImGui::DockSpaceOverViewport();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin("Viewport");
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetGUILayer()->SetBlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportpannelsize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((Eigen::Vector2f*)&viewportpannelsize)&& viewportpannelsize.x>0&& viewportpannelsize.y>0)
			m_ViewportSize = { viewportpannelsize.x,viewportpannelsize.y };

		AQUINT textureid = m_FrameBuffer->GetID();
		ImGui::Image((void*)textureid, ImVec2{m_ViewportSize.x(),m_ViewportSize.y()}, ImVec2{0,1}, ImVec2{1,0});
		ImGui::End();
		ImGui::PopStyleVar();

		if (open)
		{
			ImGui::Begin("StyleEditor", &open);
			ImGui::ShowStyleEditor();
			ImGui::End();
		}

		ImGui::Begin("Settings");
		ImGui::Text("ViewportSize: %.3f,  %.3f", m_ViewportSize.x(), m_ViewportSize.y());
		ImGui::End();


		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); it)
		{
			(*--it)->OnImGuiRender();
		}






		/*ImGui::Text("viewportsize: %f, %f", m_ViewportSize.x, m_ViewportSize.y);

		ImGui::Text("相机角度%f", m_CameraController.GetCamera().GetRotation());
		ImGui::Text("相机位置（%f，%f）", m_CameraController.GetCamera().GetPosition().x, m_CameraController.GetCamera().GetPosition().y);
		ImGui::Text("ParticleIndex:%d", m_Particle2D.GetParticleIndex());
		ImGui::Text("Alive Particle Count:%d", m_Particle2D.GetAliveParticleCount());
		ImGui::End();*/

	}

	bool LyraEditor::OnKeyPressed(KeyPressedEvent& event)
	{
		return false;
	}
	bool LyraEditor::OnWindowResize(WindowResizeEvent& event)
	{
		return false;
	}


	void LyraEditor::LyraEditor_ImGui_Style_Black(ImGuiStyle* dst)
	{
		ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
		ImVec4* colors = style->Colors;

		style->WindowMenuButtonPosition = ImGuiDir_None;
		style->FrameRounding = 12.0f;
		style->GrabRounding = 12.0f;
		style->TabRounding = 12.0f;
		style->WindowBorderSize = 0.0f;
		style->WindowPadding = ImVec2(0.00f, 0.00f);
		style->FramePadding = ImVec2(5.00f, 5.00f);
		style->ItemSpacing = ImVec2(8.00f, 8.00f);
		style->ItemInnerSpacing = ImVec2(0.00f, 0.00f);


		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.784f, 0.784f, 0.55f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.117f, 0.117f, 0.117f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.156f, 0.156f, 0.156f, 0.784f);
		colors[ImGuiCol_Border] = ImVec4(1.00f, 0.50f, 0.00f, 0.70f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.39f, 0.39f, 0.47f, 0.19f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.55f, 0.55f, 0.55f, 0.40f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(1.00f, 1.00f, 0.98f, 0.40f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.156f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.450f, 0.666f, 0.333f, 0.80f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.20f, 0.49f, 0.117f, 0.80f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.00f, 1.00f, 0.00f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 0.00f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.47f, 0.47f,0.50f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.47f, 0.47f, 0.00f, 0.70f);
		colors[ImGuiCol_Button] = ImVec4(0.80f, 0.80f, 0.80f, 0.156f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(1.00f, 1.00f, 0.00f, 0.30f);
		colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 0.62f, 1.00f, 0.45f);
		colors[ImGuiCol_Header] = ImVec4(1.00f, 1.00f, 0.00f, 0.235f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(1.00f, 0.50f, 0.00f, 0.60f);
		colors[ImGuiCol_HeaderActive] = ImVec4(1.00f, 0.50f, 0.00f, 0.60f);
		colors[ImGuiCol_Separator] = ImVec4(1.00f, 0.50f, 0.00f, 0.70f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.35f, 0.35f, 0.35f, 0.17f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		colors[ImGuiCol_Tab] = ImVec4(0.666f, 1.00f, 0.55f, 0.274f);
		colors[ImGuiCol_TabHovered] = ImVec4(1.00f, 0.50f, 0.00f, 0.60f);
		colors[ImGuiCol_TabActive] = ImVec4(1.00f, 0.50f, 0.00f, 0.47f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.666f, 1.00f, 0.55f, 0.274f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.666f, 1.00f, 0.55f, 0.274f);
		colors[ImGuiCol_DockingPreview] = colors[ImGuiCol_Header] * ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.45f, 0.00f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.78f, 0.87f, 0.98f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.57f, 0.57f, 0.64f, 1.00f);   // Prefer using Alpha=1.0 here
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.68f, 0.68f, 0.74f, 1.00f);   // Prefer using Alpha=1.0 here
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.30f, 0.30f, 0.30f, 0.09f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		colors[ImGuiCol_NavHighlight] = colors[ImGuiCol_HeaderHovered];
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
	}

	void LyraEditor::ImguiDocking(bool* p_open)
	{
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}


		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;


		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", p_open, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		else
		{
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Options"))
			{

				ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
				ImGui::MenuItem("Padding", NULL, &opt_padding);
				ImGui::Separator();

				if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
				if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
				if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
				if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
				if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
				ImGui::Separator();

				if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
					*p_open = false;
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();
	}









}