#pragma once
#include "Renderer/RenderPass.h"
#include "Scene/Scene.h"

namespace Walker {

	enum class RenderPassType {
		None = 0,
		ShadowMapPass,
		GBufferPBRPass,
		DeferredPBRLightingPass,
		BoxBlurPass,
		DepthOfFieldPass,
		EditorPass,
		BloomCompute,
		TonemapCompute
	};

	struct RenderPassSpec {
		RenderPassSpec() = default;
		RenderPassSpec(RenderPassType type, std::string name)
			: Type(type), Name(name) {}

		RenderPassType Type;
		std::string Name;
	};

	struct RenderPassLink {
		RenderPassLink() = default;
		RenderPassLink(std::string renderPassOut, std::string renderPassOutputName, std::string renderPassIn, std::string renderPassInputName)
			: RenderPassOut(renderPassOut), RenderPassOutputName(renderPassOutputName), RenderPassIn(renderPassIn), RenderPassInputName(renderPassInputName) {}

		std::string RenderPassOut;
		std::string RenderPassOutputName;
		std::string RenderPassIn;
		std::string RenderPassInputName;
	};

	struct RenderGraphSpecification {
		RenderGraphSpecification() = default;
		RenderGraphSpecification(std::initializer_list<RenderPassSpec> renderPasses, std::initializer_list<RenderPassLink> links)
			: RenderPasses(renderPasses), Links(links) {}

		std::vector<RenderPassSpec> RenderPasses;
		std::vector<RenderPassLink> Links;
	};

	class RenderGraph {
	public:
		RenderGraph(RenderGraphSpecification spec, uint32_t viewportWidth, uint32_t viewportHeight);
		~RenderGraph();

		uint32_t GetOutputRendererId() const;

		//void AddRenderPass(RenderPass renderPass);
		std::shared_ptr<RenderPass> GetRenderPass(std::string name);
		std::vector<std::shared_ptr<RenderPass>> GetRenderPasses() { return m_RenderPasses; }

		void DrawScene(Scene& scene) const;
		void Link(RenderPassOutput output, RenderPassInput input);

		void Resize(uint32_t width, uint32_t height);

		uint32_t GetWidth() const { return m_ViewportWidth; }
		uint32_t GetHeight() const { return m_ViewportHeight; }

	private:
		std::vector<std::shared_ptr<RenderPass>> m_RenderPasses;
		std::unordered_map <std::string, std::shared_ptr<RenderPass>> m_NameToPass;
		uint32_t m_ViewportWidth, m_ViewportHeight;
	};
}