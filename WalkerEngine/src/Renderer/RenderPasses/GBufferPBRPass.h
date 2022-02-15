#pragma once
#include "Renderer/Framebuffer.h"
#include "Renderer/RenderPass.h"
#include "Renderer/Shader.h"

// Temprorary
#include "Scene/Model.h"
#include "Scene/Scene.h"

namespace Walker {

	class GBufferPBRPass : RenderPass {
	public:
		GBufferPBRPass(uint32_t width, uint32_t height);
		virtual void BindInputs() const override;
		virtual void BindOutput(uint32_t outputSlot, uint32_t inputSlot) const override;
		virtual void Draw() const override;
		virtual std::vector<RenderPassInput> GetInputs() const override { return m_Inputs; };
		// Temporary
		void DrawModel(Model& model, glm::mat4 view, glm::mat4 projection) const;
		void DrawScene(Scene& scene) const;
		uint32_t GetFramebufferId() const { return m_Framebuffer->GetId(); }
		void BindTextures();

	private:
		std::shared_ptr<Framebuffer> m_Framebuffer;
		std::shared_ptr<Shader> m_Shader;
		std::vector<RenderPassInput> m_Inputs = {};
		std::vector<RenderPassOutput> m_Outputs = { {"gPosition", 0, this }, {"gNormal", 1, this }, {"gAlbedo", 2, this }, { "gMetRoughAO", 3, this } };
	};
}