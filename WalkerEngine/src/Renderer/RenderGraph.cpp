#include "walkerpch.h"
#include "RenderGraph.h"

#include "Renderer/RenderPasses/GBufferPBRPass.h"
#include "Renderer/RenderPasses/DeferredPBRLightingPass.h"

namespace Walker {

	RenderGraph::RenderGraph(uint32_t viewportWidth, uint32_t viewportHeight)
		: m_ViewportWidth(viewportWidth), m_ViewportHeight(viewportHeight)
	{
		m_RenderPasses = std::vector<std::shared_ptr<RenderPass>>();
		std::shared_ptr<RenderPass> gBufferPBRPass = std::make_shared<GBufferPBRPass>(m_ViewportWidth, m_ViewportHeight);
		std::shared_ptr<RenderPass> deferredPBRLightingPass = std::make_shared<DeferredPBRLightingPass>(m_ViewportWidth, m_ViewportHeight);
		Link(gBufferPBRPass->GetOutput("gPosition"), deferredPBRLightingPass->GetInput("gPosition"));
		Link(gBufferPBRPass->GetOutput("gNormal"), deferredPBRLightingPass->GetInput("gNormal"));
		Link(gBufferPBRPass->GetOutput("gAlbedo"), deferredPBRLightingPass->GetInput("gAlbedo"));
		Link(gBufferPBRPass->GetOutput("gMetRoughAO"), deferredPBRLightingPass->GetInput("gMetRoughAO"));

		m_RenderPasses.push_back(gBufferPBRPass);
		m_RenderPasses.push_back(deferredPBRLightingPass);
	}

	RenderGraph::~RenderGraph()
	{
	}
	/*void RenderGraph::AddRenderPass(RenderPass renderPass)
	{
	}*/
	/*std::vector<RenderPass> RenderGraph::GetRenderPasses()
	{
		return m_RenderPasses;
	}*/

	void RenderGraph::DrawScene(Scene& scene) const
	{
		// TODO: Determine draw order based on linked inputs/outputs
		for (auto renderPass : m_RenderPasses) {
			renderPass->DrawScene(scene);
		}
	}
	void RenderGraph::Link(RenderPassOutput output, RenderPassInput input)
	{
		input.RenderPass->LinkToInput(input.Name, output);
	}
}
