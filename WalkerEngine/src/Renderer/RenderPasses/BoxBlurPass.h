#pragma once
#include "Renderer/Framebuffer.h"
#include "Renderer/RenderPass.h"
#include "Renderer/Shader.h"

#include "Scene/Scene.h"

#include "Math/SampleGeometry/Quad.h"

namespace Walker {

	class BoxBlurPass : public RenderPass {
	public:
		BoxBlurPass(uint32_t width, uint32_t height);
		virtual void BindInputs() const override;
		virtual void BindOutput(uint32_t outputSlot, uint32_t inputSlot) const override;
		virtual void Draw() const override;
		virtual std::vector<RenderPassInput> GetInputs() const override { return m_Inputs; };
		virtual RenderPassInput GetInput(std::string name) const override;
		virtual RenderPassOutput GetOutput(std::string name) const override;

		virtual void LinkToInput(std::string inputName, RenderPassOutput output) override;

		virtual void DrawScene(Scene& scene) const override;

		virtual uint32_t GetFinalOutputRendererId() const override;

	private:
		std::shared_ptr<Framebuffer> m_Framebuffer;
		std::shared_ptr<Shader> m_Shader;
		std::vector<RenderPassInput> m_Inputs = { {"u_ColorTexture", 0, this } };
		std::vector<RenderPassOutput> m_Outputs = { {"gColor", 0, this } };
		std::unordered_map<std::string, RenderPassOutput> m_Links;
		Quad m_Quad;
	};
}