#pragma once
#include "Core.h"
#include "NagiEngine/Event/Event.h"
#include "Window.h"
#include "NagiEngine/ImGui/ImGuiLayer.h"
#include "LayerStack.h"

#include "Timestep.h"

#include "NagiEngine/Renderer/VertexArray.h"
#include "NagiEngine/Renderer/Shader.h"
#include "NagiEngine/Renderer/Buffer.h"
#include "NagiEngine/Renderer/OrthographicCamera.h"

namespace Nagi {

	class NAGI_API Application
	{
	public:
		Application();
		virtual ~Application(); // ����������������ͬʱ��������

		void Run();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		void OnEvent(Event& e);

		static Application& Get() { return *instance_; }
		Window& GetWindow() { return *window_; }
	private:
		bool OnWindowClose(class WindowCloseEvent& e);
		bool OnWindowResize(class WindowResizeEvent& e);
	private:
		std::unique_ptr<Window>window_;
		ImGuiLayer* imgui_layer_;
		bool running_ = true;
		bool minimized_ = false; 
		LayerStack layer_stack_;

		Timestep timestep_;
		float last_frame_time_ = 0.0f;
	private:
		static Application* instance_;
	};

	// ���ڿͻ��˱�����
	Application* CreateApplication();
}

