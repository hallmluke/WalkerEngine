#pragma once
#include "Renderer/RenderPass.h"
#include "Scene/Scene.h"

namespace Walker {

	class RenderGraph {
	public:
		RenderGraph(uint32_t viewportWidth, uint32_t viewportHeight);
		~RenderGraph();

		uint32_t GetOutputRendererId() const;

		//void AddRenderPass(RenderPass renderPass);
		std::vector<std::shared_ptr<RenderPass>> GetRenderPasses() { return m_RenderPasses; }

		void DrawScene(Scene& scene) const;
		void Link(RenderPassOutput output, RenderPassInput input);

		void Resize(uint32_t width, uint32_t height);

		uint32_t GetWidth() const { return m_ViewportWidth; }
		uint32_t GetHeight() const { return m_ViewportHeight; }

	private:
		std::vector<std::shared_ptr<RenderPass>> m_RenderPasses;
		//std::vector<std::unique_ptr<Scene>> m_R;
		uint32_t m_ViewportWidth, m_ViewportHeight;
	};
}