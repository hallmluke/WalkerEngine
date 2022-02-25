#include "SceneHierarchyPanel.h"
#include <imgui.h>
#include <imgui_internal.h>

#include "Scene/Components.h"

namespace Walker {

	SceneHierarchyPanel::SceneHierarchyPanel(const std::shared_ptr<Scene> scene)
	{
		SetContext(scene);
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		m_Context->m_Registry.each([&](auto entityID) {
			Entity entity{ entityID, m_Context.get() };

			auto& rc = entity.GetComponent<RelationshipComponent>();

			if (!rc.Parent) {
				DrawEntityNode(entity);
			}
			
		});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
			m_SelectionContext = {};
		}

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				m_Context->CreateEntity("Empty Entity");
			}

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext) {
			DrawComponents(m_SelectionContext);

			if (ImGui::Button("Add Component")) {
				ImGui::OpenPopup("AddComponent");
			}

			if (ImGui::BeginPopup("AddComponent")) {
				if (ImGui::MenuItem("Point Light")) {
					m_SelectionContext.AddComponent<PointLightComponent>();
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		if (!entity.ValidEntity() || !entity.HasComponent<TagComponent>()) {
			// Invalid/null component
			return;
		}
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		auto& relation = entity.GetComponent<RelationshipComponent>();

		ImGuiTreeNodeFlags select = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0);
		ImGuiTreeNodeFlags arrow = ((relation.Children > 0) ? ImGuiTreeNodeFlags_OpenOnArrow : ImGuiTreeNodeFlags_Leaf);
		ImGuiTreeNodeFlags flags = select | arrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked()) {
			m_SelectionContext = entity;
		}

		bool entityDeleted = false;

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity")) {
				entityDeleted = true;
			}

			ImGui::EndPopup();
		}

		if (opened) {

			if (relation.Children > 0 && relation.First) {
				DrawEntityNode(relation.First);
			}
			ImGui::TreePop();
		}
		

		if (relation.Next) {
			DrawEntityNode(relation.Next);
		}

		if (entityDeleted) {
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity) {
				m_SelectionContext = {};
			}
		}
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = ImGui::GetFont()->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());

			if (ImGui::InputText("Tag", buffer, sizeof(buffer))) {
				tag = std::string(buffer);
			}
		}

		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
				auto& translation = entity.GetComponent<TransformComponent>().Translation;
				DrawVec3Control("Translation", translation);
				auto& compRotation = entity.GetComponent<TransformComponent>().Rotation;
				glm::vec3 rotation = glm::degrees(compRotation);
				DrawVec3Control("Rotation", rotation);
				compRotation = glm::radians(rotation);
				auto& scale = entity.GetComponent<TransformComponent>().Scale;
				DrawVec3Control("Scale", scale, 1.0f);
				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<MeshComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(MeshComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Mesh")) {
				auto& mesh = entity.GetComponent<MeshComponent>().MeshPtr;
				auto& mat = mesh->GetMaterial();
				ImVec2 texSize{ 50, 50 };

				ImGui::Text("Material");
				if (mat && mat->GetAlbedo()) {
					ImGui::ImageButton((void*)mat->GetAlbedo()->GetRendererID(), texSize);
				}

				if (mat && mat->GetNormal()) {
					ImGui::ImageButton((void*)mat->GetNormal()->GetRendererID(), texSize);
				}
				
				ImGui::TreePop();
				
			}
		}

		if (entity.HasComponent<PointLightComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(PointLightComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Point Light")) {
				auto& pointLight = entity.GetComponent<PointLightComponent>().PointLightPtr;
				
				ImGui::DragFloat("Ambient Intensity", &(pointLight->m_AmbientIntensity), 0.1f, 0.0f, 3.0f);
				ImGui::DragFloat("Diffuse Intensity", &(pointLight->m_DiffuseIntensity), 0.1f, 0.0f, 3.0f);
				ImGui::DragFloat("Specular Intensity", &(pointLight->m_SpecularIntensity), 0.1f, 0.0f, 3.0f);

				ImGui::TreePop();

			}
		}
	}

	void SceneHierarchyPanel::SetContext(const std::shared_ptr<Scene>& scene)
	{
		m_Context = scene;
	}

}