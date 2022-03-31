#pragma once
#include "Renderer/RenderPass.h"
#include "Scene/Scene.h"

namespace Walker {

	class ShadowMapPass : public RenderPass {
	public:
		ShadowMapPass();
		virtual void BindInputs() const override;
		virtual void BindOutput(uint32_t outputSlot, uint32_t inputSlot) const override;
		virtual void BindOutputImage(uint32_t outputSlot, uint32_t inputSlot) const override;
		virtual std::vector<RenderPassInput> GetInputs() const override { return m_Inputs; };
		virtual RenderPassInput GetInput(std::string name) const override;
		virtual RenderPassOutput GetOutput(std::string name) const override;

		virtual void LinkToInput(std::string inputName, RenderPassOutput output) override;
		virtual void DrawScene(Scene& scene) const override;
		virtual std::shared_ptr<Framebuffer> GetFramebuffer() const override;

		virtual uint32_t GetFinalOutputRendererId() const override;

		virtual void Resize(uint32_t width, uint32_t height) override;

	private:
		std::vector<RenderPassInput> m_Inputs = {};
		// TODO: For shadow map pass, outputs should by dynamic based on the scene
		std::vector<RenderPassOutput> m_Outputs = {};
	};
}