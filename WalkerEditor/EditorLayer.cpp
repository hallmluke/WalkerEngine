#include "EditorLayer.h"
#include "imgui.h"

#include "Scene/SceneSerializer.h"
#include "Utils/PlatformUtils.h"

namespace Walker {

	EditorLayer::EditorLayer() : Layer("EditorLayer")
	{
	}
	void EditorLayer::OnAttach()
	{
		m_ActiveScene = std::make_shared<Scene>();
		//std::shared_ptr<Model> sponzaPBR = std::make_shared<Model>("SponzaPBR", "Models/SponzaPBR/Sponza.gltf", m_ActiveScene.get());

		RenderGraphSpecification spec(
			{ // Passes
				{ RenderPassType::EditorPass, "EditorPass"}
				//{RenderPassType::ShadowMapPass, "ShadowMapPass"},
				//{RenderPassType::GBufferPBRPass, "GBufferPBRPass"},
				//{RenderPassType::DeferredPBRLightingPass, "DeferredPBRLightingPass"},
				//{RenderPassType::BoxBlurPass, "BoxBlurPass"},
				//{RenderPassType::DepthOfFieldPass, "DepthOfFieldPass"} 
			},
			{ // Links
				//{ "GBufferPBRPass", "gPosition", "DeferredPBRLightingPass", "gPosition" },
				//{ "GBufferPBRPass", "gNormal", "DeferredPBRLightingPass", "gNormal" },
				//{ "GBufferPBRPass", "gAlbedo", "DeferredPBRLightingPass", "gAlbedo" },
				//{ "GBufferPBRPass", "gMetRoughAO", "DeferredPBRLightingPass", "gMetRoughAO" },
				//{ "DeferredPBRLightingPass", "gColor", "BoxBlurPass", "u_ColorTexture" },
				//{ "GBufferPBRPass", "gPosition", "DepthOfFieldPass", "u_Position" },
				//{ "DeferredPBRLightingPass", "gColor", "DepthOfFieldPass", "u_InFocusColor" },
				//{ "BoxBlurPass", "gColor", "DepthOfFieldPass", "u_OutOfFocusColor" } 
			});

		m_RenderGraph = std::make_shared<RenderGraph>(spec, 1280, 720);

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		m_MaterialPanel.SetContext(m_ActiveScene);
	}
	void EditorLayer::OnDetach()
	{
	}
	void EditorLayer::OnUpdate(float ts)
	{
		if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(m_ViewportSize.x != m_RenderGraph->GetWidth() || m_ViewportSize.y != m_RenderGraph->GetHeight())) {
			m_RenderGraph->Resize(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->GetCamera()->ResizeViewport(m_ViewportSize.x, m_ViewportSize.y);
		}
		m_ActiveScene->OnUpdate(ts);
		m_RenderGraph->DrawScene(*m_ActiveScene);

		//m_RenderGraph->GetRenderPass("EditorPass")->GetFramebuffer()->ClearAttachment(1, -1);

		/*auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = m_RenderGraph->GetRenderPass("EditorPass")->GetFramebuffer()->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
		}

		W_CORE_TRACE("Mouse: {0}, {1}", mouseX, mouseY);*/
		


		RenderCommand::BindDefaultFramebuffer();
	}
	void EditorLayer::OnImGuiRender()
	{
		// Note: Switch this to true to enable dockspace
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New")) {
					NewScene();
				}

				if (ImGui::MenuItem("Open...")) {
					OpenScene();
				}

				if (ImGui::MenuItem("Save as...")) {
					SaveSceneAs();
				}

				if (ImGui::MenuItem("Exit")) {
					// TODO Application close
					//Application::Get().Close
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_SceneHierarchyPanel.OnImGuiRender();
		m_MaterialPanel.OnImGuiRender();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize.x = viewportPanelSize.x;
		m_ViewportSize.y = viewportPanelSize.y;

		//W_CORE_TRACE("X: {0}, Y: {1}", viewportPanelSize.x, viewportPanelSize.y);

		
		uint32_t textureId = m_RenderGraph->GetOutputRendererId();
		ImGui::Image((void*)textureId, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_ActiveScene->GetCamera()->OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(EditorLayer::OnKeyPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == Mouse::ButtonLeft) {
			auto [mx, my] = ImGui::GetMousePos();
			mx -= m_ViewportBounds[0].x;
			my -= m_ViewportBounds[0].y;
			glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
			my = viewportSize.y - my;
			int mouseX = (int)mx;
			int mouseY = (int)my;

			if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
			{
				m_RenderGraph->GetRenderPass("EditorPass")->GetFramebuffer()->Bind();
				int pixelData = m_RenderGraph->GetRenderPass("EditorPass")->GetFramebuffer()->ReadPixel(1, mouseX, mouseY);
				m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
				m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
				W_CORE_TRACE("Mouse: {0}, {1}", mouseX, mouseY);
				W_CORE_TRACE("Hovered entity: {0}", (uint32_t)m_HoveredEntity);
			}
		}
		
		return false;
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = std::make_shared<Scene>();
		m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		m_MaterialPanel.SetContext(m_ActiveScene);
	}
	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Walker Scene (*.walker)\0*.walker\0");
		if (!filepath.empty())
			OpenScene(filepath);
	}
	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		if (path.extension().string() != ".walker")
		{
			W_WARN("Could not load {0} - not a scene file", path.filename().string());
			return;
		}

		std::shared_ptr<Scene> newScene = std::make_shared<Scene>();
		SceneSerializer serializer(newScene);
		if (serializer.Deserialize(path.string()))
		{
			m_ActiveScene = newScene;
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);
			m_MaterialPanel.SetContext(m_ActiveScene);
		}
	}
	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Walker Scene (*.walker)\0*.walker\0");
		if (!filepath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(filepath);
		}
	}
}