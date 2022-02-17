#include "walkerpch.h"
#include "Application.h"
#include "ImGuiManager.h"
#include "Renderer/Renderer.h"

#include <glad/glad.h>

// Temporary, for testing
#include "Scene/Model.h"
#include "Scene/Camera.h"
#include "Renderer/RenderPasses/GBufferPBRPass.h"
#include "Math/SampleGeometry/Quad.h"
#include "Renderer/Shader.h"
#include "Scene/DirectionalLight.h"
#include "Scene/PointLight.h"
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
		//DirectionalLight dirLight(glm::vec3(0.0f, -1.0f, -0.1f));
		//PointLight pointLight(glm::vec3(1.0f));
		ImGuiManager imguiManager((GLFWwindow*)m_Window->GetNativeWindow());
		//GBufferPBRPass gBufferPBRPass(m_Window->GetWidth(), m_Window->GetHeight());
		RenderGraph renderGraph(m_Window->GetWidth(), m_Window->GetHeight());
		Quad quad;
		//std::vector<PointLight*> lights = { &pointLight };
		//Skybox skybox("Skybox/default");
		Scene scene;
		Model sponzaPBR("SponzaPBR", "Models/SponzaPBR/Sponza.gltf", &scene);
		//Model nano("Nano", "Models/nano/nano_hierarchy.gltf", &scene);
		
		std::shared_ptr<Shader> shader = Shader::Create("debug", "Shaders/debug_quad.vert", "Shaders/debug_quad_input.frag");
		std::shared_ptr<Shader> shader2 = Shader::Create("debug", "Shaders/deferred_shading.vert", "Shaders/pbr_deferred.frag");

		while (m_Running) {

			RenderCommand::Clear();

			float time = (float)glfwGetTime();
			float timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;
			imguiManager.BeginFrame();

			scene.EntityDebugPanel();

			//dirLight.GenerateCascadedShadowMap(scene);
			//pointLight.GenerateShadowMap(scene);

			//gBufferPBRPass.DrawScene(scene);
			
			/*glBindFramebuffer(GL_FRAMEBUFFER, 0);

			shader2->Bind();
			shader2->SetInt("gPosition", 0);
			shader2->SetInt("gNormal", 1);
			shader2->SetInt("gAlbedo", 2);
			shader2->SetInt("gMetRoughAO", 3);
			shader2->SetVec3("camPos", scene.GetCamera()->GetPosition());
			//shader2->SetDirectionalLightProperties(dirLight, *(scene.GetCamera()));
			//shader2->SetPointLightProperties(lights);
			gBufferPBRPass.BindTextures();
			quad.Draw();

			glBindFramebuffer(GL_READ_FRAMEBUFFER, gBufferPBRPass.GetFramebufferId());
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
			glBlitFramebuffer(0, 0, m_Window->GetWidth(), m_Window->GetHeight(), 0, 0, m_Window->GetWidth(), m_Window->GetHeight(),
				GL_DEPTH_BUFFER_BIT, GL_NEAREST);*/

			//skybox.Draw(scene.GetCamera()->GetViewMatrix(), scene.GetCamera()->GetProjectionMatrix());

			//pointLight.ControlWindow();

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