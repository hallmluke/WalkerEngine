#include "walkerpch.h"
#include "Application.h"
#include "ImGuiManager.h"
#include "Model.h"

#include "GLFW/glfw3.h"

namespace Walker {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		s_Instance = this;
	}

	Application::~Application()
	{
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		//W_CORE_TRACE("{0}", e.ToString());
	}

	void Application::Run()
	{
		ImGuiManager imguiManager((GLFWwindow*)m_Window->GetNativeWindow());
		Model sponzaPBR("SponzaPBR", "Models/SponzaPBR/Sponza.gltf", glm::mat4(1.0f));

		while (m_Running) {

			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			imguiManager.BeginFrame();
			imguiManager.EndFrame();

			m_Window->OnUpdate();

		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}