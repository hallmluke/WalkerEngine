#pragma once
#include "Renderer/RenderPass.h"
#include "Renderer/Shader.h"
#include "Renderer/ComputeShader.h"
#include "Renderer/ShaderStorageBuffer.h"
#include "Scene/Scene.h"
#include "Scene/Volume.h"

namespace Walker {

	class VoxelVisualizationPass : public RenderPass {
	public:
		VoxelVisualizationPass();
		virtual void BindInputs() const override;
		virtual void BindOutput(uint32_t outputSlot, uint32_t inputSlot) const override;
		virtual void BindOutputImage(uint32_t outputSlot, uint32_t inputSlot) const override;

		virtual void DrawScene(Scene& scene) const override;

		virtual std::shared_ptr<Framebuffer> GetFramebuffer() const override;

		virtual uint32_t GetFinalOutputRendererId() const override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual void OnImGuiRender() override;

	private:
		std::shared_ptr<Volume> m_Volume;
		std::shared_ptr<Shader> m_VisualizationShader;
	};
}