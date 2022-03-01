#include "MaterialPanel.h"

#include <imgui.h>

namespace Walker {

	MaterialPanel::MaterialPanel(const std::shared_ptr<Scene> scene)
	{
		SetContext(scene);
	}

	void MaterialPanel::OnImGuiRender()
	{
		ImGui::Begin("Materials");

		std::vector<std::shared_ptr<Material>> materials = m_Context->m_MaterialLibrary.GetMaterials();

		for (auto& mat : materials) {
			bool opened = ImGui::TreeNodeEx(mat->GetName().c_str(), ImGuiTreeNodeFlags_OpenOnArrow);

			if (opened) {
				ImGui::TreePop();
			}
		}

		ImGui::End();
	}

	void MaterialPanel::SetContext(const std::shared_ptr<Scene>& scene)
	{
		m_Context = scene;
	}
}