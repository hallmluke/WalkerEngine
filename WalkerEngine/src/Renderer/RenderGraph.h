#pragma once
#include "Renderer/RenderPass.h"
#include "Scene/Scene.h"

namespace Walker {

	class RenderGraph {
	public:
		RenderGraph(uint32_t viewportWidth, uint32_t viewportHeight);
		~RenderGraph();

		//void AddRenderPass(RenderPass renderPass);
		std::vector<std::shared_ptr<RenderPass>> GetRenderPasses() { return m_RenderPasses; }

		void DrawScene(Scene& scene) const;
		void Link(RenderPassOutput output, RenderPassInput input);

	private:
		std::vector<std::shared_ptr<RenderPass>> m_RenderPasses;
		std::vector<std::unique_ptr<Scene>> m_R;
		uint32_t m_ViewportWidth, m_ViewportHeight;
	};
}