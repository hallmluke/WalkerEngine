#pragma once
#include "Renderer/Framebuffer.h"
#include "Renderer/RenderPass.h"
#include "Renderer/Shader.h"

#include "Scene/Scene.h"

namespace Walker {

	class EditorPass : public RenderPass {
	public:
		EditorPass(uint32_t width, uint32_t height);
		virtual void BindInputs() const override;
		virtual void BindOutput(uint32_t outputSlot, uint32_t inputSlot) const override;
		virtual void Draw() const override;
		virtual std::vector<RenderPassInput> GetInputs() const override { return m_Inputs; };
		virtual RenderPassInput GetInput(std::string name) const override;
		virtual RenderPassOutput GetOutput(std::string name) const override;

		virtual void LinkToInput(std::string inputName, RenderPassOutput output) override;

		virtual void DrawScene(Scene& scene) const override;
		virtual std::shared_ptr<Framebuffer> GetFramebuffer() const override;
		uint32_t GetFramebufferId() const { return m_Framebuffer->GetId(); }
		void BindTextures();

		virtual uint32_t GetFinalOutputRendererId() const override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		void ReadPixel(uint32_t index, int32_t mouseX, int32_t mouseY);

	private:
		std::shared_ptr<Framebuffer> m_Framebuffer;
		std::shared_ptr<Shader> m_Shader;
		std::vector<RenderPassInput> m_Inputs = {};
		std::vector<RenderPassOutput> m_Outputs = { {"gColor", 0, this }, {"gEntities", 1, this } };
	};
}