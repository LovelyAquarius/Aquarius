#include "Aquarius.h"

class  ExampleLayer :public Aquarius::Layer
{
public:
	ExampleLayer();

	virtual void OnUpdate(Aquarius::DeltaTime& dt)override;
	virtual void	 OnRender()override;
	virtual void OnEvent(Aquarius::BaseEvent& event)override;
	virtual void  OnImGuiRender()override;

private:
	//独立事件
	bool CameraKeyControl(Aquarius::KeyPressedEvent& event);
	bool CameraMouseControl(Aquarius::MouseMovedEvent& event);

private:
	//一些附件
	Aquarius::AQShaderLibrary m_Shaderlib;
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
	glm::vec3 m_Object_2_color{ 0.2f,0.3f,0.8f };
};
 
 
float vertices[] =
{
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,     1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,     1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,    0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,   1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,     1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,     1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,   1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,   0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

unsigned int indices[] = 
{
		 0,  1,   2, 
		 3,  4, 5,
		 6,  7,  8,
		 9,  10,  11,
	    12,  13,  14,
	    15,  16,  17,
		18  ,19, 20,
		21,22,23,
		24,25,26,
		27,28,29,
		30,31,32,
		33,34,35
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
 int framecount = 0;
 glm::vec4 red(0.8f, 0.2f, 0.3f, 1.0f);
 glm::vec4 blue(0.2f, 0.3f, 0.8f, 1.0f);


 Aquarius::ThreeDModel model;
 Aquarius::AQRef<Aquarius::AQVertexArray> box_vao ;
 Aquarius::AQRef<Aquarius::AQVertexBuffer> box_vbo ;
 Aquarius::AQRef<Aquarius::AQElementBuffer> box_ebo  ;
 Aquarius::AQRef<Aquarius::AQVertexArray> square_vao ;
 Aquarius::AQRef<Aquarius::AQVertexBuffer> square_vbo  ;
 Aquarius::AQRef<Aquarius::AQElementBuffer> square_ebo;
 Aquarius::AQGLTexture2D* pic = nullptr;
 Aquarius::AQGLTexture2D* bilibili = nullptr;

 ExampleLayer::ExampleLayer() :Layer("Example"), m_CameraController(1280.0f/720.0f), m_Object_1_position(0.0f,0.0f,0.0f)
 {
	 Aquarius::AQBufferLayout boxlayout{
			Aquarius::BufferElement {"vertex",0,Aquarius::BufferDataType::Float3},
			Aquarius::BufferElement{"texture_coords",1,Aquarius::BufferDataType::Float2}
	 };
	 Aquarius::AQBufferLayout squarelayout{
			Aquarius::BufferElement {"vertex",0,Aquarius::BufferDataType::Float3}};


	 const char* vspath1 = R"(G:\Mine\CppProjects\Aquarius\AquariusCore\source\Data\Shader\3DShader\3D.vs)";
	 const char* fspath1 = R"(G:\Mine\CppProjects\Aquarius\AquariusCore\source\Data\Shader\3DShader\3D.fs)";
	 const char* sdpath1 = R"(G:\Mine\CppProjects\Aquarius\AquariusCore\source\Data\Shader\3DShader\box.sd)";

	 const char* vspath2 = R"(G:\Mine\CppProjects\Aquarius\AquariusCore\source\Data\Shader\VS.vs)";
	 const char* fspath2 = R"(G:\Mine\CppProjects\Aquarius\AquariusCore\source\Data\Shader\FS.fs)";
	 const char* sdpath2 = R"(G:\Mine\CppProjects\Aquarius\AquariusCore\source\Data\Shader\square.sd)";
	 m_Shaderlib.Load(sdpath1);
	 m_Shaderlib.Load(sdpath2);


	 square_vao = Aquarius::AQVertexArray::Create(squarelayout);
	 square_vbo = Aquarius::AQVertexBuffer::Create(sizeof(square_data), square_data, GL_STATIC_DRAW, *square_vao);
	 square_ebo = Aquarius::AQElementBuffer::Create(sizeof(square_element), square_element, GL_STATIC_DRAW, *square_vao);
	 square_vao->Enable(0);

	 box_vao = Aquarius::AQVertexArray::Create(boxlayout);
	 box_vbo= Aquarius::AQVertexBuffer::Create(sizeof(vertices), vertices, GL_STATIC_DRAW, *box_vao);
	 box_ebo = Aquarius::AQElementBuffer::Create(sizeof(indices), indices, GL_STATIC_DRAW, *box_vao);
	 box_vao->Enable(0, 1);

	 const char* picf = R"(G:\Mine\CppProjects\Aquarius\AquariusCore\source\Data\bilibili.png)";
	 const char* bilifile = R"(G:\Mine\CppProjects\Aquarius\AquariusCore\source\Data\awesomeface.png)";
	 Aquarius::AQGLTexture2D* pic = new Aquarius::AQGLTexture2D(picf);
	 Aquarius::AQGLTexture2D* bilibili = new Aquarius::AQGLTexture2D(bilifile);
	 
	 m_Shaderlib.Get("box")->Bind();
	 ((Aquarius::AQGLShader*)((Aquarius::AQShader*)m_Shaderlib.Get("box")))->SetUniformVar("texture1", 0);
	 ((Aquarius::AQGLShader*)((Aquarius::AQShader*)m_Shaderlib.Get("box")))->SetUniformVar("texture2", 1);

	 pic->BindTextureUnit(GL_TEXTURE0);
	 bilibili->BindTextureUnit(GL_TEXTURE1);
 }

 void ExampleLayer::OnUpdate(Aquarius::DeltaTime& dt)
 {
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

 void	 ExampleLayer::OnRender()
 {
	 Aquarius::RenderCommand::SetClearcolor({ 0.1f, 0.1f, 0.1f, 1.0f });
	 Aquarius::RenderCommand::Clear();

	 //渲染camera
	 Aquarius::Renderer::BeginScene(m_CameraController.GetCamera());
	 glm::mat4 box_transform = glm::translate(glm::mat4(1.0f), m_Object_1_position);
	 box_transform = glm::rotate(box_transform, glm::radians(m_Object_1_Rotation), glm::vec3(1.0f, 0.0f, 1.0f));
	 glm::mat4 square_transform(1.0f);
	 glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_Object_2_scale);

	 m_Shaderlib.Get("square")->Bind();
	 ((Aquarius::AQGLShader*)((Aquarius::AQShader*)m_Shaderlib.Get("square")))->SetUniformVar("u_color", m_Object_2_color);


	 for (int y = 0; y < 20; y++)
	 {
		 for (int x = 0; x < 20; x++)
		 {
			 glm::vec3 pos(x * 0.12f, y * 0.12f, 0.0f);
			 square_transform = glm::translate(glm::mat4(1.0f), pos)* scale;
			 Aquarius::Renderer::Submit(square_vao, m_Shaderlib.Get("square"), square_transform);
		 }
	 }
	
	 Aquarius::Renderer::Submit(box_vao, m_Shaderlib.Get("box"), box_transform);

	 Aquarius::Renderer::EndScene();
	 //_________________________________________
 }

 
 void ExampleLayer::OnEvent(Aquarius::BaseEvent& event)
 {
	 Aquarius::EventDispatcher dispatcher(event);
	 m_CameraController.OnEvent(event);
 }
 

 void  ExampleLayer::OnImGuiRender()
 {
	 ImGui::Begin("Settings");
	 ImGui::ColorEdit3("color", glm::value_ptr(m_Object_2_color));
	 ImGui::End();
  }





 bool ExampleLayer::CameraKeyControl(Aquarius::KeyPressedEvent& event)
 {
	 return false;
 }
 bool ExampleLayer::CameraMouseControl(Aquarius::MouseMovedEvent& event)
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