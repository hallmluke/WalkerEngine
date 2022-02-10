#include "walkerpch.h"
#include "Application.h"
#include "ImGuiManager.h"
#include "Renderer/Renderer.h"

// Temporary, for testing
#include "Model.h"
#include "Camera.h"
#include "GBufferPBR.h"
#include "Quad.h"
#include "Renderer/Shader.h"
#include "DirectionalLight.h"

#include "Input.h"
#include "MouseCodes.h"

#include "GLFW/glfw3.h"

namespace Walker {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
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
		DirectionalLight dirLight(glm::vec3(0.0f, -1.0f, -0.1f));
		ImGuiManager imguiManager((GLFWwindow*)m_Window->GetNativeWindow());
		Model sponzaPBR("SponzaPBR", "Models/SponzaPBR/Sponza.gltf", glm::mat4(1.0f));
		Camera camera(glm::vec3(1.0f), m_Window->GetWidth(), m_Window->GetHeight());
		GBufferPBRPass gBufferPBRPass(m_Window->GetWidth(), m_Window->GetHeight());
		Quad quad;
		
		std::shared_ptr<Shader> shader = Shader::Create("debug", "Shaders/debug_quad.vert", "Shaders/debug_quad_input.frag");
		std::shared_ptr<Shader> shader2 = Shader::Create("debug", "Shaders/deferred_shading.vert", "Shaders/pbr_deferred.frag");

		while (m_Running) {

			RenderCommand::Clear();

			float time = (float)glfwGetTime();
			float timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;
			imguiManager.BeginFrame();

			//shader2->Bind();
			//float aspect = m_Window->GetWidth() / m_Window->GetHeight();
			//glm::mat4 view = glm::lookAt(glm::vec3(3.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			//glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
			//shader2->SetMat4("view", camera.GetViewMatrix());
			//shader2->SetMat4("projection", camera.GetProjectionMatrix());
			
			//sponzaPBR.Draw(shader2);
			dirLight.GenerateCascadedShadowMap(sponzaPBR, camera);

			gBufferPBRPass.DrawModel(sponzaPBR, camera.GetViewMatrix(), camera.GetProjectionMatrix());
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			/*shader2->Bind();
			shader2->SetMat4("view", camera.GetViewMatrix());
			shader2->SetMat4("projection", camera.GetProjectionMatrix());

			sponzaPBR.Draw(shader2);*/

			shader2->Bind();
			shader2->SetInt("gPosition", 0);
			shader2->SetInt("gNormal", 1);
			shader2->SetInt("gAlbedo", 2);
			shader2->SetInt("gMetRoughAO", 3);
			shader2->SetVec3("camPos", camera.GetPosition());
			shader2->SetDirectionalLightProperties(dirLight, camera);
			gBufferPBRPass.BindTextures();
			quad.Draw();


			/*glBindFramebuffer(GL_READ_FRAMEBUFFER, gBufferPBRPass.GetFramebufferId());
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
			glBlitFramebuffer(0, 0, m_Window->GetWidth(), m_Window->GetHeight(), 0, 0, m_Window->GetWidth(), m_Window->GetHeight(),
				GL_DEPTH_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);*/
			if (Input::IsMouseButtonPressed(Mouse::ButtonRight)) {
				m_Window->DisableCursor();
			}
			else {
				m_Window->EnableCursor();
			}
			camera.OnUpdate(timestep);

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