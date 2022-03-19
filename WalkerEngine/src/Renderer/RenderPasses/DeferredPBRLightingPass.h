#pragma once
#include "Renderer/Framebuffer.h"
#include "Renderer/RenderPass.h"
#include "Renderer/Shader.h"

#include "Scene/Scene.h"
#include "Scene/DirectionalLight.h"
#include "Scene/PointLight.h"

#include "Math/SampleGeometry/Quad.h"

namespace Walker {

	class DeferredPBRLightingPass : public RenderPass {
	public:
		DeferredPBRLightingPass(uint32_t width, uint32_t height);
		virtual void BindInputs() const override;
		virtual void BindOutput(uint32_t outputSlot, uint32_t inputSlot) const override;
		virtual void BindOutputImage(uint32_t outputSlot, uint32_t inputSlot) const override;
		virtual std::vector<RenderPassInput> GetInputs() const override { return m_Inputs; };
		virtual RenderPassInput GetInput(std::string name) const override;
		virtual RenderPassOutput GetOutput(std::string name) const override;

		virtual void LinkToInput(std::string inputName, RenderPassOutput output) override;

		virtual uint32_t GetFinalOutputRendererId() const override;

		virtual void DrawScene(Scene& scene) const override;
		virtual std::shared_ptr<Framebuffer> GetFramebuffer() const override;
		uint32_t GetFramebufferId() const { return m_Framebuffer->GetId(); }
		void BindTextures();

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual void OnImGuiRender() override;

	private:
		std::shared_ptr<Framebuffer> m_Framebuffer;
		std::shared_ptr<Shader> m_Shader;
		std::vector<RenderPassInput> m_Inputs = { {"gPosition", 0, this }, {"gNormal", 1, this }, {"gAlbedo", 2, this }, { "gMetRoughAO", 3, this }, { "VoxelTex", 4, this } };
		std::vector<RenderPassOutput> m_Outputs = { {"gColor", 0, this } };
		std::unordered_map<std::string, RenderPassOutput> m_Links;
		Quad m_Quad;

		bool m_UseVoxelConetrace = false;
		float m_Aperture = 0.57;
		float m_MaxDistance = 100.0f;
		float m_StepSize = 1.0f;
		float m_IndirectMultiplier = 1.0f;
		int32_t m_NumCones = 8;


		void SetDirectionalLightShaderUniforms(Scene& scene) const;
		void SetPointLightShaderUniforms(Scene& scene) const;
	};
}