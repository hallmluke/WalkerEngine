#include "walkerpch.h"
#include "Application.h"
#include "ImGuiManager.h"
#include "Renderer/Renderer.h"

#include <glad/glad.h>

#include "Scene/Model.h"
#include "Scene/Skybox.h"
#include "Scene/Scene.h"
#include "Renderer/RenderGraph.h"

#include "Input.h"
#include "MouseCodes.h"

#include "GLFW/glfw3.h"


namespace Walker {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		WindowProps props;
		props.Icon = "Icons/skywalker_icon.png";

		m_Window = std::unique_ptr<Window>(Window::Create(props));
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		s_Instance = this;

		Renderer::Init();
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
		RenderGraph renderGraph(m_Window->GetWidth(), m_Window->GetHeight());
		//Skybox skybox("Skybox/default");
		Scene scene;
		Model sponzaPBR("SponzaPBR", "Models/SponzaPBR/Sponza.gltf", &scene);

		while (m_Running) {

			RenderCommand::Clear();

			float time = (float)glfwGetTime();
			float timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;
			imguiManager.BeginFrame();

			scene.EntityDebugPanel();
			renderGraph.DrawScene(scene);
			
			/*
			TODO: Render graph support for copying depth buffer
			glBindFramebuffer(GL_READ_FRAMEBUFFER, gBufferPBRPass.GetFramebufferId());
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
			glBlitFramebuffer(0, 0, m_Window->GetWidth(), m_Window->GetHeight(), 0, 0, m_Window->GetWidth(), m_Window->GetHeight(),
				GL_DEPTH_BUFFER_BIT, GL_NEAREST);*/

			//skybox.Draw(scene.GetCamera()->GetViewMatrix(), scene.GetCamera()->GetProjectionMatrix());

			if (Input::IsMouseButtonPressed(Mouse::ButtonRight)) {
				m_Window->DisableCursor();
			}
			else {
				m_Window->EnableCursor();
			}
			scene.OnUpdate(timestep);

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