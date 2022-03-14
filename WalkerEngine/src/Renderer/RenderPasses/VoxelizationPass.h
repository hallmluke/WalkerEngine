#pragma once
#include "Renderer/RenderPass.h"
#include "Renderer/Shader.h"
#include "Renderer/ComputeShader.h"
#include "Scene/Scene.h"
#include "Scene/Volume.h"

namespace Walker {

	class VoxelizationPass : public RenderPass {
	public:
		VoxelizationPass();
		virtual void BindInputs() const override;
		virtual void BindOutput(uint32_t outputSlot, uint32_t inputSlot) const override;
		virtual void BindOutputImage(uint32_t outputSlot, uint32_t inputSlot) const override;

		virtual void DrawScene(Scene& scene) const override;

		virtual std::shared_ptr<Framebuffer> GetFramebuffer() const override;

		virtual uint32_t GetFinalOutputRendererId() const override;

		virtual void Resize(uint32_t width, uint32_t height) override;

	private:
		mutable bool m_Cache;
		std::shared_ptr<Texture3D> m_VoxelTex;
		std::shared_ptr<Shader> m_VoxelizationShader;
		std::shared_ptr<ComputeShader> m_VolumeCompShader;
		std::shared_ptr<Volume> m_Volume;
		std::shared_ptr<Shader> m_VisualizationShader;

		void SetDirectionalLightShaderUniforms(Scene& scene) const;
		void SetPointLightShaderUniforms(Scene& scene) const;
	};
}