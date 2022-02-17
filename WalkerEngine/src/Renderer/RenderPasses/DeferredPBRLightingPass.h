#pragma once

#pragma once
#include "Renderer/Framebuffer.h"
#include "Renderer/RenderPass.h"
#include "Renderer/Shader.h"

// Temprorary
#include "Scene/Model.h"
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
		virtual void Draw() const override;
		virtual std::vector<RenderPassInput> GetInputs() const override { return m_Inputs; };
		virtual RenderPassInput GetInput(std::string name) const override;
		virtual RenderPassOutput GetOutput(std::string name) const override;

		virtual void LinkToInput(std::string inputName, RenderPassOutput output) override;

		virtual void DrawScene(Scene& scene) const override;
		uint32_t GetFramebufferId() const { return m_Framebuffer->GetId(); }
		void BindTextures();

	private:
		std::shared_ptr<Framebuffer> m_Framebuffer;
		std::shared_ptr<Shader> m_Shader;
		std::vector<RenderPassInput> m_Inputs = { {"gPosition", 0, this }, {"gNormal", 1, this }, {"gAlbedo", 2, this }, { "gMetRoughAO", 3, this } };
		std::vector<RenderPassOutput> m_Outputs = { {"color", 0, this } };
		std::unordered_map<std::string, RenderPassOutput> m_Links;
		Quad m_Quad;

		void SetDirectionalLightShaderUniforms(Scene& scene) const;
		void SetPointLightShaderUniforms(Scene& scene) const;
	};
}