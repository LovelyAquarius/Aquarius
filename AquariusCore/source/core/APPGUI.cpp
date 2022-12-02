#include "AQPCH.h"
#include "APPGUI.h"
#include "Application.h"
#include"Platform/AQOpenGL/GLError.h"
#include"ImGuizmo.h"
#include <GLFW/glfw3.h>
#include<IconsFontAwesome/IconsFontAwesome6.h>
#include<IMGUI/imgui_internal.h>
namespace Aquarius
{
	void APPGUI::OnAttach()
	{

        IMGUI_CHECKVERSION();

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;    
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;        
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;      

        ImGui::StyleColorsLight();
        const char* defaultfont = R"(.\source\Assets\Internal\Fonts\msyh.ttc)";
        io.Fonts->AddFontFromFileTTF(defaultfont, 24.0f, 0, io.Fonts->GetGlyphRangesChineseFull());

        static const ImWchar icon_ranges[] = 
        { 0x23fb,0x23fb,
          ICON_MIN_FA, ICON_MAX_FA, 0 
        };

        ImFontConfig config;
        config.MergeMode = true;
        config.GlyphMinAdvanceX = 24.0f;

        const char* iconfont = R"(G:\Mine\CppProjects\Aquarius\Lyra\source\Assets\Internal\Icons\fa-regular-400.ttf)";
        io.Fonts->AddFontFromFileTTF(iconfont, 24.0f, &config, icon_ranges);
        io.Fonts->AddFontFromFileTTF("./source/Assets/Internal/Icons/fa-solid-900.ttf", 24.0f, &config, icon_ranges);


        ImGuiStyle& style = ImGui::GetStyle();

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        Application& app = Application::Get();
        GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

        const char* glsl_version = "#version 410";
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);


        


	}

	void APPGUI::OnDetach()
	{
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
	}

    void APPGUI::OnImGuiRender()
    {
        static bool show = true;

    }


    


    void APPGUI::Begin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
        ImGui::GetCurrentContext()->NavWindowingToggleLayer = false;
    }

    void APPGUI::End()
    {

        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

    }

    void APPGUI::OnEvent(BaseEvent& event)
    {
        if (m_BlockEvents)
        {
            ImGuiIO& io = ImGui::GetIO();
            event.m_Handled |= event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
            event.m_Handled |= event.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
        }

    }







	bool APPGUI::OnSpaceButtonPressed(KeyPressedEvent& event)
	{
		if(event.GetKeyCode()== AQ_KEY_SPACE)
			AQ_CORE_INFO("我按下了空格！");
		return false;
	}







}