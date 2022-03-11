#include "walkerpch.h"
#include "RenderGraph.h"

#include "Renderer/RenderPasses/ShadowMapPass.h"
#include "Renderer/RenderPasses/GBufferPBRPass.h"
#include "Renderer/RenderPasses/DeferredPBRLightingPass.h"
#include "Renderer/RenderPasses/BoxBlurPass.h";
#include "Renderer/RenderPasses/DepthOfFieldPass.h"
#include "Renderer/RenderPasses/EditorPass.h"
#include "Renderer/RenderPasses/BloomComputePass.h"
#include "Renderer/RenderPasses/TonemapComputePass.h"

namespace Walker {

	namespace Utils {
		static std::shared_ptr<RenderPass> RenderPassTypeToRenderPass(RenderPassType type, uint32_t viewportWidth, uint32_t viewportHeight) {
			switch (type) {
			case RenderPassType::ShadowMapPass: return std::make_shared<ShadowMapPass>();
			case RenderPassType::GBufferPBRPass: return std::make_shared<GBufferPBRPass>(viewportWidth, viewportHeight);
			case RenderPassType::DeferredPBRLightingPass: return std::make_shared<DeferredPBRLightingPass>(viewportWidth, viewportHeight);
			case RenderPassType::BoxBlurPass: return std::make_shared<BoxBlurPass>(viewportWidth, viewportHeight);
			case RenderPassType::DepthOfFieldPass: return std::make_shared<DepthOfFieldPass>(viewportWidth, viewportHeight);
			case RenderPassType::EditorPass: return std::make_shared<EditorPass>(viewportWidth, viewportHeight);
			case RenderPassType::BloomCompute: return std::make_shared<BloomComputePass>(viewportWidth, viewportHeight);
			case RenderPassType::TonemapCompute: return std::make_shared<TonemapComputePass>(viewportWidth, viewportHeight);
			}

			return nullptr;
		}
	}

	RenderGraph::RenderGraph(RenderGraphSpecification spec, uint32_t viewportWidth, uint32_t viewportHeight)
		: m_ViewportWidth(viewportWidth), m_ViewportHeight(viewportHeight)
	{
		m_RenderPasses = std::vector<std::shared_ptr<RenderPass>>();

		for (auto& renderPassSpec : spec.RenderPasses) {
			auto renderPass = Utils::RenderPassTypeToRenderPass(renderPassSpec.Type, m_ViewportWidth, m_ViewportHeight);
			m_RenderPasses.push_back(renderPass);
			m_NameToPass[renderPassSpec.Name] = renderPass;
		}

		for (auto& link : spec.Links) {
			Link(m_NameToPass[link.RenderPassOut]->GetOutput(link.RenderPassOutputName), m_NameToPass[link.RenderPassIn]->GetInput(link.RenderPassInputName));
		}
	}

	RenderGraph::~RenderGraph()
	{
	}

	uint32_t RenderGraph::GetOutputRendererId() const
	{
		auto LastRenderPass = m_RenderPasses[m_RenderPasses.size() - 1];
		return LastRenderPass->GetFinalOutputRendererId();
	}

	std::shared_ptr<RenderPass> RenderGraph::GetRenderPass(std::string name)
	{
		return m_NameToPass[name];
	}

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
	void RenderGraph::Resize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		for (auto renderPass : m_RenderPasses) {
			renderPass->Resize(width, height);
		}
	}
}
