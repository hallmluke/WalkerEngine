#pragma once
#include "Scene/Scene.h"
#include "Renderer/Material.h"

namespace Walker {

	class MaterialPanel {
	public:
		MaterialPanel() = default;
		MaterialPanel(const std::shared_ptr<Scene> scene);

		void OnImGuiRender();

		void SetContext(const std::shared_ptr<Scene>& scene);
	private:
		std::shared_ptr<Scene> m_Context;
	};
}