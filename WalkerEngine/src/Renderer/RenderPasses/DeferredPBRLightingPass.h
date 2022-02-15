#pragma once

#pragma once
#include "Renderer/Framebuffer.h"
#include "Renderer/RenderPass.h"
#include "Renderer/Shader.h"

// Temprorary
#include "Scene/Model.h"
#include "Scene/Scene.h"

#include "Math/SampleGeometry/Quad.h"

namespace Walker {

	class DeferredPBRLightingPass : RenderPass {
	public:
		DeferredPBRLightingPass(uint32_t width, uint32_t height);
		virtual void BindInputs() const override;
		virtual void Draw() const override;
		virtual std::vector<RenderPassInput> GetInputs() const override { return m_Inputs; };
		virtual RenderPassOutput GetOutput(std::string name) const override;

		virtual void LinkToInput(std::string inputName, RenderPassOutput output) override;

		void DrawScene(Scene& scene) const;
		uint32_t GetFramebufferId() const { return m_Framebuffer->GetId(); }
		void BindTextures();

	private:
		std::shared_ptr<Framebuffer> m_Framebuffer;
		std::shared_ptr<Shader> m_Shader;
		std::vector<RenderPassInput> m_Inputs = { {"gPosition", 0, this }, {"gNormal", 1, this }, {"gAlbedo", 2, this }, { "gMetRoughAO", 3, this } };
		std::unordered_map<std::string, RenderPassOutput> m_Links;
		Quad m_Quad;
	};
}