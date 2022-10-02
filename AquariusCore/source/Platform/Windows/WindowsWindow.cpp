#include"AQPCH.h"

#include "Platform/Windows/WindowsWindow.h"
#include"EventSystem/ApplicationEvent.h"
#include "EventSystem/KeyEvent.h"
#include "EventSystem/MouseEvent.h"
#include "core/Log.h"


namespace Aquarius
{

	static bool s_GLFWInitialized = false;


	static void GLFWErrorCallback(int error, const char* descripition)
	{
		AQ_CORE_ERROR("GLFW ERROR ({0}) : {1}", error, descripition);
	}




	Window* Window::Create(const WindowConfigs& configs)
	{
		return new WindowsWindow(configs);
	}


	WindowsWindow::WindowsWindow(const WindowConfigs& configs)
	{
		Init(configs);
	}

	WindowsWindow::~WindowsWindow()
	{
		ShutDown();
	}


	void WindowsWindow::Init(const WindowConfigs& configs)
	{
		m_Data.Title = configs.Title;
		m_Data.Width = configs.Width;
		m_Data.Height = configs.Height;
		AQ_CORE_INFO("Creating Window:{0}({1}, {2})", configs.Title, configs.Width, configs.Height);

	
		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			AQ_CORE_ASSERT(success, "Could not initialize GLFW");
	
			glfwSetErrorCallback(GLFWErrorCallback);

			s_GLFWInitialized = true;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



		m_Window = glfwCreateWindow((int)configs.Width, (int)configs.Height, m_Data.Title.c_str(), nullptr, nullptr);
		AQ_CORE_ASSERT(m_Window, "Could not initialize GLFW window!");

		m_Context = new AQGLContext(m_Window);
		m_Context->Init();


		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetSync(true);



		glfwSetWindowSizeCallback(m_Window,
			[](GLFWwindow* window, int width, int height)
			{
				
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				data.Width = width;
				data.Height = height;
				WindowResizeEvent event(width, height);
				data.EventCallBack(event);
			});




		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				WindowCloseEvent event;
				data.EventCallBack(event);
			});

	

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window,
			int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallBack(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallBack(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallBack(event);
					break;
				}
				}
			});

		glfwSetMouseButtonCallback(m_Window,
			[](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallBack(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallBack(event);
					break;
				}
				}
			}
		);


		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				KeyTypedEvent event(keycode);
				data.EventCallBack(event);

			});




		glfwSetScrollCallback(m_Window,
			[](GLFWwindow* window, double xOffset, double yOffset)
			{

				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallBack(event);
			});


		glfwSetCursorPosCallback(m_Window,
			[](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((float)xPos, (float)yPos);
				data.EventCallBack(event);
			});


	}



	void WindowsWindow::ShutDown()
	{
		glfwDestroyWindow(m_Window);
	}


	void WindowsWindow::OnUpdade()
	{

		glfwPollEvents();
		m_Context->SwapBuffers();

	}


	void WindowsWindow::SetSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		m_Data.VSync = enabled;
	}


	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}


}