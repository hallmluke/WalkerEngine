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
		virtual void BindOutput(uint32_t outputSlot, uint32_t inputSlot) const override;
		virtual void BindOutputImage(uint32_t outputSlot, uint32_t inputSlot) const override;

		virtual void DrawScene(Scene& scene) const override;

		virtual std::shared_ptr<Framebuffer> GetFramebuffer() const override;

		virtual uint32_t GetFinalOutputRendererId() const override;

		virtual void Resize(uint32_t width, uint32_t height) override;

	private:
		std::shared_ptr<Framebuffer> m_Framebuffer;
		std::shared_ptr<Shader> m_Shader;
		Quad m_Quad;
	};
}