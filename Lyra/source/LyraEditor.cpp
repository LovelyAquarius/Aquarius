#include "LyraPCH.h"
#include "LyraEditor.h"
#include "core/Translation.h"
#include "core/APPGUI.h"
#include "Utils/PlatformUtils.h"
#include "Scene/SceneSerializer.h"
#include "ImGuizmo.h"
#include "UISystem/IMGUILayer/AQImguiLibrary.h"
#include "Scene/ElementSystem/AQComponents/AQComponentHeader.h"
#include "ScriptSystem/CoreScripts/AQCameraController.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <IMGUI/imgui_internal.h>
#include <IconsFontAwesome/IconsFontAwesome6.h>
#pragma warning(disable : 4312)
namespace Aquarius
{
	Translation LyraEditor::s_Translation = Translation::English;
	
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


		m_ActiveScene = AQScene::Create("InitialScene");
		m_ContentBrowser = new LyraContentBrowser();
		m_SceneHierarchy = new LyraSceneHierarchy(m_ActiveScene);

		//设置子项的栈
		PushOverLay(m_ContentBrowser); //ContentBrowser      1
		PushOverLay(m_SceneHierarchy);//SceneHierarchy       2



		const char* bilifile = R"(G:\Mine\CppProjects\Aquarius\AquariusCore\source\Data\bilibili.png)";
		const char* picf = R"(G:\Mine\CppProjects\Aquarius\AquariusCore\source\Data\Checkerboard.png)";
		const char* mypicfile = R"(G:\Mine\CppProjects\Aquarius\AquariusCore\source\Data\lovely.png)";
		backpicture = AQTexture2D::Create(picf);
		bilibilipicture = AQTexture2D::Create(bilifile);
		mypicture = AQTexture2D::Create(mypicfile);
		//_____________________________________-
		spirite = AQSubTexture2D::Create(bilibilipicture, { 162,124 }, { 1,1 });
		const int unicode = 0xf190;

		std::string  nn = AQ_UnicodeToUTF8((wchar_t*) & unicode);

	}

	LyraEditor::~LyraEditor()
	{
		m_LayerStack.DestroyLayers();
	}

	void LyraEditor::OnAttach()
	{
		//ImGui初始化
		LyraEditor_ImGui_Style_Black();

		//framebuffer config
		AQFrameBufferConfiguration fbconfig1;
		fbconfig1.Width = 1280;
		fbconfig1.Height = 720;
		fbconfig1.Attachment = { FrameBufferTexutureFormat::RGBA8,FrameBufferTexutureFormat::REDI,FrameBufferTexutureFormat::DEPTH };
		m_FrameBuffer = AQFrameBuffer::Create(fbconfig1);


		//background scene
		m_BackScene = AQScene::Create("BackScene");
		m_SquareEntity = m_BackScene->CreateEntity("square");
		m_SquareEntity.AddComponent<AQSpiriteComponent>(Eigen::Vector4f().setConstant(1.0f));


		//EditorCamera
		m_EditorCamera = EditorCamera(30.0f, 1.778f, 1.0f, 1000.0f);
		m_StaticCamera = EditorCamera(45.0f, 1.778f, 1.0f, 1000.0f);
		m_StaticCamera.SetViewportSize((float)Application::Get().GetWindow().GetWidth(), (float)Application::Get().GetWindow().GetHeight());
		//entity config
		

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

	
		}

		//____________________________________________
		//相机和场景更新
		if (m_ViewportFocused)
		{
			m_EditorCamera.OnUpdate(dt);
			m_CameraController.OnUpdate(dt);
		}
		m_BackScene->OnUpdate(dt); 
		m_ActiveScene->OnUpdate(dt);
		// camera控制
		
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

			AQFrameBufferConfiguration config = m_FrameBuffer->GetConfiguration();
			if (m_ViewportSize.x() > 0.0f && m_ViewportSize.y() > 0.0f && (config.Width != m_ViewportSize.x() || config.Height != m_ViewportSize.y()))
			{
				m_ActiveScene->OnViewportResize((AQUINT)m_ViewportSize.x(), (AQUINT)m_ViewportSize.y());
				m_EditorCamera.SetViewportSize(m_ViewportSize.x(), m_ViewportSize.y());
				m_FrameBuffer->Resize((AQUINT)m_ViewportSize.x(), (AQUINT)m_ViewportSize.y());
			}

			auto [mousex, mousey] = ImGui::GetMousePos();
			mousex -= m_ViewportBounds[0].x();
			mousey -= m_ViewportBounds[0].y();
			mousey = m_ViewportSize.y() - mousey;
			int mousexi = (int)mousex, mouseyi = (int)mousey;

			m_FrameBuffer->Bind();
			//初始化渲染
			RenderCommand::SetClearcolor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RenderCommand::Clear();
			//__________________________________________________
			m_FrameBuffer->ClearAttachment(1, -1);
			m_ActiveScene->OnRenderEditor(dt,m_EditorCamera);
			if (mousexi > 0 && mouseyi > 0 && mousexi < (int)m_ViewportSize.x() && mouseyi < (int)m_ViewportSize.y())
			{
				auto pixeldata = m_FrameBuffer->ReadPixel(1, mousexi, mouseyi);
				m_HoveredEntity = pixeldata == -1 ? AQEntity() : AQEntity((entt::entity)pixeldata, m_ActiveScene);
				/*AQ_CORE_INFO("pixeldata:{0}", pixeldata);*/
			}
			m_FrameBuffer->UnBind();


		}

		{
			//初始化渲染
			RenderCommand::SetClearcolor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RenderCommand::Clear();
			//__________________________________________________
			m_ActiveScene->OnRenderEditor(dt, m_StaticCamera);
		}
	}


	void LyraEditor::OnEvent(BaseEvent& event)
	{

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(AQ_BIND_EVENT_FN(LyraEditor::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(AQ_BIND_EVENT_FN(LyraEditor::OnMouseButtonPressed));
		dispatcher.Dispatch<WindowResizeEvent>(AQ_BIND_EVENT_FN(LyraEditor::OnWindowResize));
		m_EditorCamera.OnEvent(event);
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
		static std::string Tranlation_Language, Tranlation_English, Tranlation_Chinese, Tranlation_File, Tranlation_SaveScene, Tranlation_LoadScene, Tranlation_Exit;
		static std::string Tranlation_Openfile, Tranlation_Save_file_as;
		static bool TranslationChanged = true;

		if (TranslationChanged)
		{
			if (s_Translation == Translation::English)
			{
				Tranlation_Language = AQ_TRANSLATION_ENGLISH_Language;
				Tranlation_File = AQ_TRANSLATION_ENGLISH_File;
				Tranlation_SaveScene = AQ_TRANSLATION_ENGLISH_SaveScene;
				Tranlation_LoadScene = AQ_TRANSLATION_ENGLISH_LoadScene;
				Tranlation_English = AQ_TRANSLATION_ENGLISH_English;
				Tranlation_Chinese = AQ_TRANSLATION_ENGLISH_Chinese;
				Tranlation_Exit = AQ_TRANSLATION_ENGLISH_Exit;
				Tranlation_Openfile = AQ_TRANSLATION_ENGLISH_Openfile;
				Tranlation_Save_file_as = AQ_TRANSLATION_ENGLISH_Save_file_as;

			}
			else if (s_Translation == Translation::Chinese)
			{
				Tranlation_Language = AQ_TRANSLATION_CHINESE_Language;
				Tranlation_File = AQ_TRANSLATION_CHINESE_File;
				Tranlation_SaveScene = AQ_TRANSLATION_CHINESE_SaveScene;
				Tranlation_LoadScene = AQ_TRANSLATION_CHINESE_LoadScene;
				Tranlation_English = AQ_TRANSLATION_CHINESE_English;
				Tranlation_Chinese = AQ_TRANSLATION_CHINESE_Chinese;
				Tranlation_Exit = AQ_TRANSLATION_CHINESE_Exit;
				Tranlation_Openfile = AQ_TRANSLATION_CHINESE_Openfile;
				Tranlation_Save_file_as = AQ_TRANSLATION_CHINESE_Save_file_as;
			}

			TranslationChanged = false;
		}
		
		ImGui::DockSpaceOverViewport(0,ImGuiDockNodeFlags_PassthruCentralNode);

		if (ImGui::BeginMainMenuBar())
		{
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.313f,0.784f,0.141f,1.0f });
			bool file_menu = ImGui::BeginMenu((std::string(ICON_FA_WINDOW_RESTORE) + " " + Tranlation_File).c_str());
			if (file_menu)
			{
				if (ImGui::MenuItem((std::string(ICON_FA_FILE_IMPORT) + "  " + Tranlation_Openfile).c_str(), "Ctrl+O")) Openfile();
				if (ImGui::MenuItem((std::string(ICON_FA_FILE_EXPORT) + " " + Tranlation_Save_file_as).c_str(), "Ctrl+Shift+S")) Savefileas();
				if (ImGui::MenuItem((std::string(ICON_U_23FB) + "  " + Tranlation_Exit).c_str())) Application::Get().Close();
				ImGui::EndMenu();
			}
	

			if (ImGui::BeginMenu((std::string(ICON_FA_GLOBE) + " " + Tranlation_Language).c_str()))
			{
				if (ImGui::MenuItem((std::string("En") + "     " + Tranlation_English).c_str())) 
				{
					s_Translation = Translation::English;
					m_ContentBrowser->SetTranslation(s_Translation);
					TranslationChanged = true;
				}
				if (ImGui::MenuItem((std::string("中") + "     " + Tranlation_Chinese).c_str())) 
				{
					s_Translation = Translation::Chinese;
					m_ContentBrowser->SetTranslation(s_Translation);
					TranslationChanged = true;
				}
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
		ImGui::PopStyleColor();







		{
			//场景视窗
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
			ImVec4 currentwindowbg = ImGui::GetStyle().Colors[2];
			currentwindowbg.w = 1.0f;
			ImGui::PushStyleColor(ImGuiCol_WindowBg, currentwindowbg);
			ImGui::Begin("Viewport");

			auto viewportminregion = ImGui::GetWindowContentRegionMin();
			auto viewportmaxregion = ImGui::GetWindowContentRegionMax();
			auto viewportoffset = ImGui::GetWindowPos();
			m_ViewportBounds[0] = { viewportminregion.x + viewportoffset.x, viewportminregion.y + viewportoffset.y };
			m_ViewportBounds[1] = { viewportmaxregion.x + viewportoffset.x, viewportmaxregion.y + viewportoffset.y };

			m_ViewportFocused = ImGui::IsWindowFocused();
			m_ViewportHovered = ImGui::IsWindowHovered();
			Application::Get().GetGUILayer()->SetBlockEvents(!m_ViewportFocused && !m_ViewportHovered);
			m_EditorCamera.m_ViewPortFocusing = m_ViewportFocused && m_ViewportHovered;

			auto viewportpannelsize = m_ViewportBounds[1]- m_ViewportBounds[0];
			if (m_ViewportSize != *((Eigen::Vector2f*)&viewportpannelsize) && viewportpannelsize.x() > 0 && viewportpannelsize.y() > 0)
				m_ViewportSize = { viewportpannelsize.x(),viewportpannelsize.y()};

			AQUINT textureid = m_FrameBuffer->GetColorAttachmentID(0);
			ImGui::Image((void*)textureid, ImVec2{ m_ViewportSize.x(),m_ViewportSize.y() }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
		}
		
		//Gizmos
		AQEntity selectedentity = m_SceneHierarchy->GetSelectedEntity();

		if (selectedentity && m_GuizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			float windowwidth = (float)ImGui::GetWindowWidth();
			float windowheight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x,ImGui::GetWindowPos().y, windowwidth, windowheight);

			bool snap = Input::IsKeyPressed(AQ_KEY_LEFT_CONTROL);
			float snapValue = 0.5f; 

			if (m_GuizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;
			float snapValues[3] = { snapValue, snapValue, snapValue };

			//主相机参数
			const Eigen::Matrix4f& cameraprojection = m_EditorCamera.GetProjection();
			const Eigen::Matrix4f& cameraview = m_EditorCamera.GetView();
			//Entity参数
			auto transformcomponent = selectedentity.GetComponent<AQTransformComponent>();
			Eigen::Transform<AQFLOAT, 3, Eigen::Affine> gtransform{ transformcomponent->GetTransform() };

			ImGuizmo::Manipulate(cameraview.data(), cameraprojection.data(), (ImGuizmo::OPERATION)m_GuizmoType, ImGuizmo::LOCAL, gtransform.data(),nullptr, snap ? snapValues : nullptr);
			if (ImGuizmo::IsUsing())
			{

				if(m_GuizmoType== ImGuizmo::OPERATION::TRANSLATE)
					transformcomponent->Translation = gtransform.translation();
				else if(m_GuizmoType == ImGuizmo::OPERATION::ROTATE)
					transformcomponent->Rotation = gtransform.rotation().eulerAngles(0, 1, 2);
				else if (m_GuizmoType == ImGuizmo::OPERATION::SCALE)
				{
					float sx = gtransform.matrix().block<3, 1>(0, 0).norm();
					float sy = gtransform.matrix().block<3, 1>(0, 1).norm();
					float sz = gtransform.matrix().block<3, 1>(0, 2).norm();
					transformcomponent->Scale << sx, sy, sz;
				}

			}
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		ImGui::End();
		//___________________________________________________________________________

		if (open)
		{
			ImGui::Begin("StyleEditor", &open);
			ImGui::ShowStyleEditor();
			ImGui::End();
		}
		ImGui::Begin("Font");
		ImGui::ShowFontSelector("Selector");
		ImGui::End();





		ImGui::Begin("Settings");
		ImGui::Text("ViewportSize: %.3f,  %.3f", m_ViewportSize.x(), m_ViewportSize.y());
		auto allviewport = m_ViewportBounds[1] - m_ViewportBounds[0];
		ImGui::Text("ViewportSize: %.3f,  %.3f", allviewport.x(), allviewport.y());
		/*ImGui::Text("EditorCamera:");

		float cameradistance = m_EditorCamera.GetDistance();
		if(ImGui::DragFloat("Distance", &cameradistance, 1.0f))
			m_EditorCamera.SetDistance(cameradistance);
		float camerafov = m_EditorCamera.GetFov();
		if (ImGui::DragFloat("Fov", &camerafov, 1.0f))
			m_EditorCamera.SetFov(camerafov);

		ImGui::Text("CameraViewProjection");
		ImGui::Text("%.3f,  %.3f,  %.3f,  %.3f", m_EditorCamera.GetViewProjection().col(0).x(), m_EditorCamera.GetViewProjection().col(1).x(), m_EditorCamera.GetViewProjection().col(2).x(), m_EditorCamera.GetViewProjection().col(3).x());
		ImGui::Text("%.3f,  %.3f,  %.3f,  %.3f", m_EditorCamera.GetViewProjection().col(0).y(), m_EditorCamera.GetViewProjection().col(1).y(), m_EditorCamera.GetViewProjection().col(2).y(), m_EditorCamera.GetViewProjection().col(3).y());
		ImGui::Text("%.3f,  %.3f,  %.3f,  %.3f", m_EditorCamera.GetViewProjection().col(0).z(), m_EditorCamera.GetViewProjection().col(1).z(), m_EditorCamera.GetViewProjection().col(2).z(), m_EditorCamera.GetViewProjection().col(3).z());
		ImGui::Text("%.3f,  %.3f,  %.3f,  %.3f", m_EditorCamera.GetViewProjection().col(0).w(), m_EditorCamera.GetViewProjection().col(1).w(), m_EditorCamera.GetViewProjection().col(2).w(), m_EditorCamera.GetViewProjection().col(3).w());*/

		//ImGui::Text("backCameraViewProjection");
		//ImGui::Text("%.3f,  %.3f,  %.3f,  %.3f", m_StaticCamera.GetViewProjection().col(0).x(), m_StaticCamera.GetViewProjection().col(1).x(), m_StaticCamera.GetViewProjection().col(2).x(), m_StaticCamera.GetViewProjection().col(3).x());
		//ImGui::Text("%.3f,  %.3f,  %.3f,  %.3f", m_StaticCamera.GetViewProjection().col(0).y(), m_StaticCamera.GetViewProjection().col(1).y(), m_StaticCamera.GetViewProjection().col(2).y(), m_StaticCamera.GetViewProjection().col(3).y());
		//ImGui::Text("%.3f,  %.3f,  %.3f,  %.3f", m_StaticCamera.GetViewProjection().col(0).z(), m_StaticCamera.GetViewProjection().col(1).z(), m_StaticCamera.GetViewProjection().col(2).z(), m_StaticCamera.GetViewProjection().col(3).z());
		//ImGui::Text("%.3f,  %.3f,  %.3f,  %.3f", m_StaticCamera.GetViewProjection().col(0).w(), m_StaticCamera.GetViewProjection().col(1).w(), m_StaticCamera.GetViewProjection().col(2).w(), m_StaticCamera.GetViewProjection().col(3).w());
		//auto ts = m_SquareEntity.GetComponent<AQTransformComponent>()->GetTransform();
		//ImGui::Text("square");
		//ImGui::Text("%.3f,  %.3f,  %.3f,  %.3f", ts.col(0).x(), ts.col(1).x(), ts.col(2).x(), ts.col(3).x());
		//ImGui::Text("%.3f,  %.3f,  %.3f,  %.3f", ts.col(0).y(), ts.col(1).y(), ts.col(2).y(), ts.col(3).y());
		//ImGui::Text("%.3f,  %.3f,  %.3f,  %.3f", ts.col(0).z(), ts.col(1).z(), ts.col(2).z(), ts.col(3).z());
		//ImGui::Text("%.3f,  %.3f,  %.3f,  %.3f", ts.col(0).w(), ts.col(1).w(), ts.col(2).w(), ts.col(3).w());
		ImGui::Text("CameraView");
		ImGui::Text("%.3f,  %.3f,  %.3f,  %.3f", m_EditorCamera.GetView().col(0).x(), m_EditorCamera.GetView().col(1).x(), m_EditorCamera.GetView().col(2).x(), m_EditorCamera.GetView().col(3).x());
		ImGui::Text("%.3f,  %.3f,  %.3f,  %.3f", m_EditorCamera.GetView().col(0).y(), m_EditorCamera.GetView().col(1).y(), m_EditorCamera.GetView().col(2).y(), m_EditorCamera.GetView().col(3).y());
		ImGui::Text("%.3f,  %.3f,  %.3f,  %.3f", m_EditorCamera.GetView().col(0).z(), m_EditorCamera.GetView().col(1).z(), m_EditorCamera.GetView().col(2).z(), m_EditorCamera.GetView().col(3).z());
		ImGui::Text("%.3f,  %.3f,  %.3f,  %.3f", m_EditorCamera.GetView().col(0).w(), m_EditorCamera.GetView().col(1).w(), m_EditorCamera.GetView().col(2).w(), m_EditorCamera.GetView().col(3).w());

		ImGui::Text("CameraPosition");
		Eigen::Vector3f camerapos = m_EditorCamera.GetPosition();
		ImGui::Text("%.3f,  %.3f,  %.3f", camerapos.x(), camerapos.y(), camerapos.z());
		/*ImGui::Text("%.3f,  %.3f,  %.3f", m_EditorCamera.GetFocalPoint().x(), m_EditorCamera.GetFocalPoint().y(), m_EditorCamera.GetFocalPoint().z());*/
		if (ImGui::DragFloat3("##CameraPosition", camerapos.data(), 0.1f))
			m_EditorCamera.SetPosition(camerapos);

		//ImGui::Text("Position");
		//Eigen::Vector4f pos = { 0.0f,0.0f,0.0f,1.0f };
		//Eigen::Vector4f vppos = m_EditorCamera.GetViewProjection() * pos;
		//ImGui::Text("%.3f,  %.3f,  %.3f,  %.3f", vppos.x(), vppos.y(), vppos.z(), vppos.w());
	
		
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
		if(event.GetRepeatCount()>0)
			return false;
		bool iscontrolpressed = Input::IsKeyPressed(AQ_KEY_LEFT_CONTROL) || Input::IsKeyPressed(AQ_KEY_RIGHT_CONTROL);
		bool isshiftpressed = Input::IsKeyPressed(AQ_KEY_LEFT_SHIFT) || Input::IsKeyPressed(AQ_KEY_RIGHT_SHIFT);
		switch (event.GetKeyCode())
		{
			case(AQ_KEY_O):
			{
				if (iscontrolpressed)
					Openfile();
				break;
			}
			case(AQ_KEY_S):
			{
				if (iscontrolpressed && isshiftpressed)
					Savefileas();
				break;
			}
			case(AQ_KEY_A):
			{
				if (iscontrolpressed)
					m_GuizmoType = -1;
				break;
			}

			case(AQ_KEY_Z):
			{
				if (iscontrolpressed)
					m_GuizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			}
			case(AQ_KEY_X):
			{
				if (iscontrolpressed)
					m_GuizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			}
			case(AQ_KEY_C):
			{
				if (iscontrolpressed)
					m_GuizmoType = ImGuizmo::OPERATION::SCALE;
				break;
			}
		}
		return false;
	}
	bool LyraEditor::OnMouseButtonPressed(MouseButtonPressedEvent& event)
	{
		if (event.GetMouseButton() == AQ_MOUSE_BUTTON_LEFT)
		{
			if (m_ViewportHovered && !ImGuizmo::IsOver() && !(Input::IsKeyPressed(AQ_KEY_LEFT_ALT) || Input::IsKeyPressed(AQ_KEY_RIGHT_ALT)))
				m_SceneHierarchy->SetSelectedEntity(m_HoveredEntity);
			//AQ_CORE_INFO("Left pressed");
		}


		return false;
	}
	bool LyraEditor::OnWindowResize(WindowResizeEvent& event)
	{
		m_StaticCamera.SetViewportSize((AQFLOAT)event.GetWidth(), (AQFLOAT)event.GetHeight());
		m_BackScene->OnViewportResize(event.GetWidth(), event.GetHeight());
		return false;
	}

	void LyraEditor::Openfile()
	{
		std::string filepath = FileDialogs::OpenFile("Aquarius Scene (*.aqscene)\0*.aqscene\0");
		if (!filepath.empty())
		{
			m_ActiveScene = AQScene::Create();
			m_ActiveScene->OnViewportResize((AQUINT)m_ViewportSize.x(), (AQUINT)m_ViewportSize.y());
			m_SceneHierarchy->SetContext(m_ActiveScene);
			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(filepath);
		}
	}

	void LyraEditor::Savefileas()
	{
		std::string filepath = FileDialogs::SaveFile("Aquarius Scene (*.aqscene)\0*.aqscene\0");
		if (!filepath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(filepath);
		}
	}


	void LyraEditor::LyraEditor_ImGui_Style_Black(ImGuiStyle* dst)
	{
		ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
		ImVec4* colors = style->Colors;

		style->WindowMenuButtonPosition = ImGuiDir_None;
		style->FrameRounding = 0.0f;
		style->GrabRounding = 12.0f;
		style->TabRounding = 2.0f;
		style->TabBorderSize = 1.0f;
		style->WindowBorderSize = 0.0f;
		style->WindowPadding = ImVec2(0.00f, 0.00f);
		style->FramePadding = ImVec2(6.00f, 5.00f);
		style->ItemSpacing = ImVec2(8.00f, 8.00f);
		style->ItemInnerSpacing = ImVec2(0.00f, 0.00f);
		

		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.784f, 0.784f, 0.55f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.117f, 0.117f, 0.117f, 0.2f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.156f, 0.156f, 0.156f, 0.784f);
		colors[ImGuiCol_Border] = ImVec4(0.47f, 0.47f, 0.47f, 0.70f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.39f, 0.39f, 0.47f, 0.19f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.55f, 0.55f, 0.55f, 0.40f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(1.00f, 1.00f, 0.98f, 0.40f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.117f, 0.117f, 0.156f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.39f, 0.39f, 0.47f, 0.19f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.55f, 0.55f, 0.40f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(1.00f, 1.00f, 0.98f, 0.40f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.00f, 1.00f, 0.98f, 0.40f);
		colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 0.00f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.47f, 0.47f,0.50f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.47f, 0.47f, 0.00f, 0.70f);
		colors[ImGuiCol_Button] = ImVec4(0.80f, 0.80f, 0.80f, 0.156f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.509f, 0.509f, 0.509f, 0.313f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.509f, 0.509f, 0.509f, 0.313f);
		colors[ImGuiCol_Header] = ImVec4(0.313f, 0.313f, 0.313f, 0.235f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
		colors[ImGuiCol_Separator] = ImVec4(0.47f, 0.47f, 0.47f, 0.70f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.35f, 0.35f, 0.35f, 0.17f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		colors[ImGuiCol_Tab] = ImVec4(1.0f, 1.0f, 1.0f, 0.117f);
		colors[ImGuiCol_TabHovered] = ImVec4(1.0f, 0.0f, 0.0f, 0.745f);
		colors[ImGuiCol_TabActive] = ImVec4(1.0f, 0.0f, 0.0f, 0.745f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.0f, 0.0f, 0.0f, 0.3f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.0f, 0.0f, 0.0f, 0.6f);
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