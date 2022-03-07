#pragma once
#include "Renderer/Framebuffer.h"
#include "Renderer/RenderPass.h"
#include "Renderer/ComputeShader.h"
#include "Scene/Scene.h"

namespace Walker {

	class BloomComputePass : public RenderPass {
	public:
		BloomComputePass(uint32_t width, uint32_t height);
		virtual void BindInputs() const override;
		virtual void BindOutput(uint32_t outputSlot, uint32_t inputSlot) const override;
		virtual void BindOutputImage(uint32_t outputSlot, uint32_t inputSlot) const override;

		virtual void DrawScene(Scene& scene) const override;

		virtual std::shared_ptr<Framebuffer> GetFramebuffer() const override;

		virtual uint32_t GetFinalOutputRendererId() const override;

		virtual void Resize(uint32_t width, uint32_t height) override;

	private:
		std::shared_ptr<Texture> m_Half;
		std::shared_ptr<Texture> m_Quarter;
		std::shared_ptr<Texture> m_Eigth;
		std::shared_ptr<ComputeShader> m_Prefilter;
		std::shared_ptr<ComputeShader> m_Downsample;
		std::shared_ptr<ComputeShader> m_Upsample;
	};
}