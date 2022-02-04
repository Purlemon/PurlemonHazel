#include "WindowsWindow.h"
#include "../../PurlemonHazel/Log.h"

#include "../../PurlemonHazel/Event/ApplicationEvent.h"
#include "../../PurlemonHazel/Event/KeyEvent.h"
#include "../../PurlemonHazel/Event/MouseEvent.h"

#include <glad/glad.h>
#include "GLFW/glfw3.h"

namespace PurlemonHazel {

	static bool sGLFWInitialized = false;

	static void GLFWErrorCallback(int error_code, const char* description)
	{
		PH_CORE_ERROR("GLFW Error ({0}): {1}", error_code, description);
	}


	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}
	
	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		data_.title = props.title;
		data_.width = props.width;
		data_.height = props.height;

		PH_CORE_INFO("���ڴ������� {0} ({1}, {2})", props.title, props.width, props.height);

		// ����Ψһ����
		if (!sGLFWInitialized) {
			int success = glfwInit();
			PH_CORE_ASSERT(success, "Could not intialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			sGLFWInitialized = true;
		}

		window_ = glfwCreateWindow((int)props.width, (int)props.height, data_.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(window_);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		PH_CORE_ASSERT(status, "Failed to initialize Glad!");
		glfwSetWindowUserPointer(window_, &data_);
		SetVSync(true);

		// ����GLFW�ص�����
		glfwSetWindowSizeCallback(window_, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.width = width;
			data.height = height;

			WindowResizeEvent event(width, height);
			data.event_callback(event); // ���ð󶨵�Application::OnEvent����
		});

		glfwSetWindowCloseCallback(window_, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.event_callback(event);
		});

		glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
				case GLFW_PRESS: 
				{
					KeyPressedEvent event(key, 0);
					data.event_callback(event);
					break;
				}
				case GLFW_RELEASE: 
				{
					KeyReleasedEvent event(key);
					data.event_callback(event);
					break;
				}
				case GLFW_REPEAT: 
				{
					KeyPressedEvent event(key, 1); // ��ʱ����
					data.event_callback(event);
					break;
				}
			}
		});

		glfwSetMouseButtonCallback(window_, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
				case GLFW_PRESS: 
				{
					MouseButtonPressedEvent event(button);
					data.event_callback(event);
					break;
				}
				case GLFW_RELEASE: 
				{
					MouseButtonReleasedEvent event(button);
					data.event_callback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(window_, [](GLFWwindow* window, double xoffset, double yoffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xoffset, (float)yoffset);
			data.event_callback(event);
		});

		glfwSetCursorPosCallback(window_, [](GLFWwindow* window, double xpos, double ypos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xpos, (float)ypos);
			data.event_callback(event);
		});
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(window_);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(window_);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled) {
			glfwSwapInterval(1); // �ȴ�ˢ�º��ٽ���������ʵ�ִ�ֱͬ��
		}
		else {
			glfwSwapInterval(0); // ���̽���������
		}

		data_.vsync = enabled;
	}

	bool WindowsWindow::IsVSync()const
	{
		return data_.vsync;
	}

}