#include "Aquarius.h"
#include "Renderer/ParticleSystem/AQParticle2D.h"
#include "Platform/AQOpenGL/AQGLBuffer.h"
#include "Utils/AQFont/AQTextImage.h"
class  ExampleLayer :public Aquarius::Layer
{
public:
	ExampleLayer();

	virtual void OnAttach()override;
	virtual void OnUpdate(Aquarius::DeltaTime& dt)override;
	virtual void	 OnRender(Aquarius::DeltaTime& dt)override;
	virtual void OnEvent(Aquarius::BaseEvent& event)override;
	virtual void  OnImGuiRender()override;

private:
	//独立事件
	bool OnKeyPressed(Aquarius::KeyPressedEvent& event);
	bool OnWindowResize(Aquarius::WindowResizeEvent& event);
private:
	//一些附件
	Aquarius::AQShaderLibrary m_Shaderlib;
	Aquarius::AQParticcle2D m_Particle2D;
	Aquarius::AQParticcle2D::ParticleProperties m_ParticleSetting;
	Aquarius::AQRef<Aquarius::AQFrameBuffer> m_FrameBuffer;
	// ————————————————————————————————————————
	//camera相关属性
	Aquarius::OrthgraphicCameraController m_CameraController;
	//_______________________
	//物体相关属性
	glm::vec3 m_Object_1_position;
	float m_Object_1_Rotation = 0.0f;
	float m_Object_1_MovingSpeed = 1.0f;
	float m_Object_1_Rotationspeed = 0.0f;

	glm::vec3 m_Object_2_scale{ 0.1f,0.1f,0.0f };
	glm::vec4 m_Object_2_color{ 0.2f,0.3f,0.8f ,1.0f};
};
 
 
float vertices[] =
{
	-0.5f, -0.5f, 0.0f,        0.5f, 0.5f, 0.0f,      -0.5f, 0.5f, 0.0f,    10.0f,    0.8f, 0.2f, 0.3f, 1.0f
};

unsigned int indices[] = 
{
	0
};

float square_data[] =
{
		0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
};

unsigned int square_element[] =
{
	  0, 1, 3,
	  1, 2, 3
};

 bool firstframe = true;
 float acceleration = 7.0f;
 float onesecondcount=0.0f;
 float fivesecondcount = 0.0f;
 float testdata;
 int framecount = 0;
 glm::vec4 red(0.8f, 0.2f, 0.3f, 1.0f);
 glm::vec4 blue(0.2f, 0.3f, 0.8f, 1.0f);
 glm::vec3 randompotision;


 Aquarius::AQRef<Aquarius::AQTexture2D> backpicture;
 Aquarius::AQRef<Aquarius::AQTexture2D> bilibilipicture;
 Aquarius::AQRef<Aquarius::AQTexture2D> mypicture;
 Aquarius::AQRef<Aquarius::AQSubTexture2D> spirite;


 glm::vec3 start = { -2.7f,-5.7f,0.0f };
 glm::vec3 end = { 0.2f,0.4f,0.0f };
 glm::vec3 controlpoint={4.3f,-2.0f,0.0f};
 float* input = new float;
 uint32_t pointcount;

 Aquarius::AQRef<Aquarius::AQVertexArray>  testvao;
 Aquarius::AQRef<Aquarius::AQVertexBuffer>  testvbo;
 Aquarius::AQRef<Aquarius::AQElementBuffer> testebo;
 Aquarius::AQRef<Aquarius::AQShader> testshader;

 glm::vec2 center = { 0.0f,0.0f };
 float radius = 1.0f;
 float magicnumber = (4.0f / 3.0f) * glm::tan(2.0f * glm::pi<float>() / 2.0f / 4.0f);
 std::vector<glm::vec2> quadraticbezier;
 glm::vec2 direction;

 std::vector<Aquarius::AQRef<Aquarius::AQQuadraticBezierCurve2D>> bezier;
 char utf8[10] = "";
 char utf8_old[5] = "";
 char unicode[10];




 ExampleLayer::ExampleLayer() :Layer("Example"), m_CameraController(1280.0f / 720.0f), m_Object_1_position(0.0f, 0.0f, 0.0f),m_Particle2D(30000)
 {
	 //const char* mshader = R"(G:\Mine\CppProjects\Aquarius\AquariusCore\source\Data\Shader\renderer2d_beziershape.glsl)";
	 //m_Shaderlib.Load(mshader);

	 const char* bilifile = R"(G:\Mine\CppProjects\Aquarius\AquariusCore\source\Data\bilibili.png)";
	 const char* picf = R"(G:\Mine\CppProjects\Aquarius\AquariusCore\source\Data\Checkerboard.png)";
	 const char* mypicfile = R"(G:\Mine\CppProjects\Aquarius\AquariusCore\source\Data\lovely.png)";
	 backpicture = Aquarius::AQTexture2D::Create(picf);
	 bilibilipicture = Aquarius::AQTexture2D::Create(bilifile);
	 mypicture = Aquarius::AQTexture2D::Create(mypicfile);


	 //设置particle
	 m_ParticleSetting.BeginColor = { 254 / 255.0f,212 / 255.0f,123 / 255.0f,1.0f };
	 m_ParticleSetting.EndColor = { 254 / 255.0f,109 / 255.0f,41 / 255.0f, 1.0f };
	 m_ParticleSetting.BeginSize = 0.3f;
	 m_ParticleSetting.SizeVariation = 0.3f;
	 m_ParticleSetting.EndSize = 0.0f;
	 m_ParticleSetting.Lifetime = 7.0f;
	 m_ParticleSetting.Velosity = { 0.0f,0.0f };
	 m_ParticleSetting.VelosityVariation = { 3.0f, 1.0f };
	 m_ParticleSetting.Position = { 0.0f,0.0f };
	 //________________________
	 strcpy(utf8, (char*)u8"");
	Aquarius::AQ_UTF8ToUnicode(utf8,unicode);

	 Aquarius::AQFontAPI::GetAQFontAPI()->LoadFont(Aquarius::AQTextImage::fontfilepath);
	 //Aquarius::AQFontAPI::bitmap* bitmap =Aquarius::AQFontAPI::GetAQFontAPI()->GetSinglecharacterBitmap(0x53, 200);
	 //Aquarius::AQFontAPI::GetAQFontAPI()->WriteBitmapToPNG("test.png", bitmap);
	 Aquarius::AQFontAPI::GetAQFontAPI()->GetSinglecharacterBezier(*(unsigned short*)unicode, bezier, 0.001f);
	 //_____________________________________-
	 spirite = Aquarius::AQSubTexture2D::Create(bilibilipicture, { 162,124 }, { 1,1 });

	 Aquarius::AQRef<Aquarius::AQSubTexture2D> copy = spirite->Copy();


 }

 void ExampleLayer::OnAttach()
 {
	 Aquarius::AQFrameBufferConfiguration fbconfig;
	 fbconfig.Width = 1280;
	 fbconfig.Height = 720;
	 m_FrameBuffer = Aquarius::AQFrameBuffer::Create(fbconfig);
 }
 void ExampleLayer::OnUpdate(Aquarius::DeltaTime& dt)
 {
	 AQ_TIME_MONITOR("ExampleLayer::OnUpdate");
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
	 if (fivesecondcount <3.0f)
	 {
		 fivesecondcount += dt;
	 }
	 else
	 {
		 float x = Aquarius::AQRandom::Float()*1280.0f;
		 float y = Aquarius::AQRandom::Float()*720.0f;
		 float r = Aquarius::AQRandom::Float() * 40;
		 auto width = Application::Get().GetWindow().GetWidth();
		 auto height = Application::Get().GetWindow().GetHeight();

		 auto bounds = m_CameraController.GetBounds();
		 auto position = m_CameraController.GetCamera().GetPosition();
		 x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		 y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		 m_ParticleSetting.Position = { x + position.x, y + position.y };
		 for (int i = 0; i < r; i++)
			 m_Particle2D.Emit(m_ParticleSetting);
	
		 fivesecondcount = 0.0f;
		 fivesecondcount += dt;
	 }
	 //____________________________________________
	 // camera控制
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
	/* if (Input::IsMouseButtonPressed(AQ_MOUSE_BUTTON_LEFT))
	 {
		 auto [x, y] = Input::GetMousePosition();
		 auto width = Application::Get().GetWindow().GetWidth();
		 auto height = Application::Get().GetWindow().GetHeight();

		 auto bounds = m_CameraController.GetBounds();
		 auto position = m_CameraController.GetCamera().GetPosition();
		 x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		 y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		 m_ParticleSetting.Position = { x + position.x, y + position.y };
		 for (int i = 0; i < 10; i++)
			 m_Particle2D.Emit(m_ParticleSetting);
	 }*/
	 m_Particle2D.OnUpdate(dt);
	 //_____________________________________
	 //object非控制
	 if (firstframe)
		 firstframe = false;
	 else
	 {
		 if (m_Object_1_Rotationspeed >100.0f)
			 acceleration *= -1.0f;
		 else if(m_Object_1_Rotationspeed<-100.0f)
			 acceleration *= -1.0f;

		 m_Object_1_Rotationspeed += acceleration * dt;
		 m_Object_1_Rotation += m_Object_1_Rotationspeed * dt;

		 if (m_Object_1_Rotation > 360.0f)
		 {
			 m_Object_1_Rotation -= 360.0f;
		 }
		 else if (m_Object_1_Rotation < -360.f)
		 {
			 m_Object_1_Rotation += 360.0f;
		 }
	 }
	 //_______________________________
 }

 void	 ExampleLayer::OnRender(Aquarius::DeltaTime& dt)
 {
	 AQ_TIME_MONITOR("ExampleLayer::OnRender");
	 //清除所有状态
	 Aquarius::Renderer2D::ResetStatistics();
	 //__________________________________________________
	 //渲染camera
	 {
		 AQ_TIME_MONITOR("Renderer2D::Scene");
		 m_FrameBuffer->Bind();
		 //初始化渲染
		 Aquarius::RenderCommand::SetClearcolor({ 0.1f, 0.1f, 0.1f, 1.0f });
		 Aquarius::RenderCommand::Clear();
		 //__________________________________________________
		 Aquarius::Renderer2D::BeginScene(m_CameraController.GetCamera());

		 if(*(unsigned short*)utf8== *(unsigned short*)utf8_old|| *(unsigned short*)utf8==0)
		 { }
		 else
		 {
			 *(unsigned short*)utf8_old = *(unsigned short*)utf8;
			 Aquarius::AQ_UTF8ToUnicode(utf8, unicode);
			 Aquarius::AQFontAPI::GetAQFontAPI()->GetSinglecharacterBezier(*(unsigned short*)unicode, bezier, 0.001f);
		 }
		
		 for (int i = 0; i < bezier.size(); i++)
		 {
			 Aquarius::Renderer2D::DrawBezier_Line_GPU(bezier[i],20, red,5.0f);
		 }
		 Aquarius::Renderer2D::EndScene();
		 m_Particle2D.OnRender(m_CameraController.GetCamera());
		 m_FrameBuffer->UnBind();
	 }
 }

 
 void ExampleLayer::OnEvent(Aquarius::BaseEvent& event)
 {
	 Aquarius::EventDispatcher dispatcher(event);
	 dispatcher.Dispatch<Aquarius::KeyPressedEvent>(AQ_BIND_EVENT_FN(ExampleLayer::OnKeyPressed));
	 m_CameraController.OnEvent(event);
 }
 

 void  ExampleLayer::OnImGuiRender()
 {
	 ImGui::Begin("Renderer2D Settings");
	 ImGui::ColorEdit4("begincolor", glm::value_ptr(m_ParticleSetting.BeginColor));
	 ImGui::ColorEdit4("endcolor", glm::value_ptr(m_ParticleSetting.EndColor));
	 ImGui::InputText("字(Unicode编码)", utf8, IM_ARRAYSIZE(utf8));


	 ImGui::Text("相机角度%f", m_CameraController.GetCamera().GetRotation());

	 ImGui::Text("相机位置（%f，%f）", m_CameraController.GetCamera().GetPosition().x, m_CameraController.GetCamera().GetPosition().y);
	 ImGui::Text("ParticleIndex:%d", m_Particle2D.GetParticleIndex());
	 ImGui::Text("Alive Particle Count:%d", m_Particle2D.GetAliveParticleCount());

	 AQUINT textureid = m_FrameBuffer->GetID();
	 ImGui::Image((void*)textureid, ImVec2{ 640.0f,360.0f }, ImVec2{0,1},ImVec2{ 1,0 });


	 auto stats = Aquarius::Renderer2D::GetStatistics();
	 ImGui::Text("Renderer2D Statistics:");
	 ImGui::Text("DrawCalls:%d", stats.DrawCalls);
	 ImGui::Text("Quads:%d", stats.QuadCount);
	 ImGui::Text("QuadVertices:%d", stats.GetQuadVertexCount());
	 ImGui::Text("QuadElements:%d", stats.GetQuadElementCount());
	 ImGui::Text("BezierVertices:%d", stats.GetBezierVertexCount());

	 ImGui::Text("性能监控:");
	 for (auto& profilemessage : Aquarius::Application::s_profile)
	 {
		 char label[70];
		 strcpy(label, profilemessage.Name);
		 strcat(label, "%s %.3fms");
		 ImGui::Text(label,"用时：", profilemessage.Time);
	 }
	 Aquarius::Application::s_profile.clear();
	 ImGui::End();
  }

 bool ExampleLayer::OnKeyPressed(Aquarius::KeyPressedEvent& event)
 {
	 if (event.GetKeyCode() == AQ_KEY_Z)
	 {
		 auto [x, y] = Aquarius::Input::GetMousePosition();
		 auto width = Aquarius::Application::Get().GetWindow().GetWidth();
		 auto height = Aquarius::Application::Get().GetWindow().GetHeight();
		 auto bounds = m_CameraController.GetBounds();
		 auto position = m_CameraController.GetCamera().GetPosition();
		 x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		 y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		 start = { x + position.x, y + position.y ,0.0f };
	 }
	 else if (event.GetKeyCode() == AQ_KEY_X)
	 {
		 auto [x, y] = Aquarius::Input::GetMousePosition();
		 auto width = Aquarius::Application::Get().GetWindow().GetWidth();
		 auto height = Aquarius::Application::Get().GetWindow().GetHeight();
		 auto bounds = m_CameraController.GetBounds();
		 auto position = m_CameraController.GetCamera().GetPosition();
		 x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		 y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		 end = { x + position.x, y + position.y ,0.0f };
	 }
	 return false;
 }
 bool ExampleLayer::OnWindowResize(Aquarius::WindowResizeEvent& event)
 {
	 return false;
 }

class  AquariusApp : public Aquarius::Application
{
public:
	AquariusApp()
	{
		PushOverLay(new ExampleLayer());
		
	}

	~AquariusApp()
	{

	}

private:
		

};




Aquarius::Application* Aquarius::CreateApplication()
{
	return new AquariusApp();
}