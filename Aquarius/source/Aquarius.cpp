#include "Aquarius.h"

class  ExampleLayer :public Aquarius::Layer
{
public:
	ExampleLayer();


	virtual void	 OnRender()override;

private:

	Aquarius::OrthgraphicCamera m_Camera;

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
};;




 float rotationspeed = 0.0f;
 float acceleration = glm::radians(0.001f);
 bool forward = true;

 Aquarius::ThreeDModel model;
 Aquarius::ThreeDModel view;
 Aquarius::ThreeDModel projection{};
 std::shared_ptr < Aquarius::AQGLShader> shaderprogram = nullptr;
 std::shared_ptr<Aquarius::AQVertexArray> VAO = nullptr;
 std::shared_ptr<Aquarius::AQVertexBuffer> VBO = nullptr;
 Aquarius::AQGLTexture2D* pic = nullptr;
 Aquarius::AQGLTexture2D* bilibili = nullptr;

 ExampleLayer::ExampleLayer() :Layer("Example"), m_Camera(-1.0f, 2.0f, -1.0f, 1.0f)
 {
	 Aquarius::AQBufferLayout mylayout{
			Aquarius::BufferElement {"vertex",0,Aquarius::BufferDataType::Float3},
			Aquarius::BufferElement{"texture_coords",1,Aquarius::BufferDataType::Float2}
	 };
	 const char* vspath2 = R"(G:\Mine\CppProjects\Aquarius\AquariusCore\source\Data\Shader\3DShader\3D.vs)";
	 const char* fspath2 = R"(G:\Mine\CppProjects\Aquarius\AquariusCore\source\Data\Shader\3DShader\3D.fs)";

	 shaderprogram = std::make_shared < Aquarius::AQGLShader>(vspath2, fspath2);

	
	 VAO = std::shared_ptr<Aquarius::AQVertexArray>(Aquarius::AQVertexArray::Create(mylayout));
	 VBO= std::shared_ptr<Aquarius::AQVertexBuffer>(Aquarius::AQVertexBuffer::Create(sizeof(vertices), vertices, GL_STATIC_DRAW, *VAO));
	 std::cout << VAO->GetLayout().GetElements()[0].Count << std::endl;
	 std::cout << VAO->GetLayout().GetElements()[1].Count << std::endl;
	 VAO->Enable(0, 1);

	 const char* picf = R"(G:\Mine\CppProjects\Aquarius\AquariusCore\source\Data\bilibili.png)";
	 const char* bilifile = R"(G:\Mine\CppProjects\Aquarius\AquariusCore\source\Data\awesomeface.png)";
	 Aquarius::AQGLTexture2D* pic = new Aquarius::AQGLTexture2D(picf, GL_RGBA, GL_TEXTURE0);
	 pic->SetAttributes(GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
	 Aquarius::AQGLTexture2D* bilibili =new Aquarius::AQGLTexture2D(bilifile, GL_RGBA, GL_TEXTURE1);
	 bilibili->SetAttributes(GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);

	 shaderprogram->Use();
	 shaderprogram->SetUniformVar("texture1", 0);
	 shaderprogram->SetUniformVar("texture2", 1);

	 //3d

	 model.Rotate((float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
	 view.Translate(glm::vec3(0.0f, 0.0f, -3.0f));
	 projection.Get() = glm::perspective(glm::radians(45.0f), (float)800 / 600, 0.1f, 100.0f);

	 //传入着色器
	 shaderprogram->SetUniformVar("model", model.Get());
	 unsigned int viewLoc = glGetUniformLocation(shaderprogram->GetShaderProgram(), "view");
	 glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view.Get()[0][0]);
	 shaderprogram->SetUniformVar("projection", projection.Get());

	 pic->BindTextureUnit(GL_TEXTURE0);
	 bilibili->BindTextureUnit(GL_TEXTURE1);

 }

 void	 ExampleLayer::OnRender()
 {
	 Aquarius::RenderCommand::SetClearcolor({ 0.2f, 0.3f, 0.3f, 1.0f });
	 Aquarius::RenderCommand::Clear();

	 m_Camera.SetPosition({ 0.0f,-0.5f,-0.1f });
	 m_Camera.SetRotation(30.0f);
	 m_Camera.CalculateView();
	 Aquarius::Renderer::BeginScene(m_Camera);

	 rotationspeed += acceleration;
	 model.Rotate(rotationspeed, glm::vec3(0.0f, 0.0f, 1.0f));

	 if (rotationspeed > glm::radians(1.0f) && forward)
	 {
		 forward = false;
		 acceleration *= -1;
	 }
	 else if (rotationspeed < glm::radians(-1.0f) && !forward)
	 {
		 forward = true;
		 acceleration *= -1;
	 }

	 shaderprogram->SetUniformVar("model", model.Get());

	 Aquarius::Renderer::Submit(VAO, shaderprogram);

	 Aquarius::Renderer::EndScene();

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