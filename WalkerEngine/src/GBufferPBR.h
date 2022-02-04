#pragma once
#include "Renderer/Framebuffer.h"
#include "Renderer/RenderPass.h"
#include "Renderer/Shader.h"

namespace Walker {

	class GBufferPBRPass : RenderPass {
	public:
		GBufferPBRPass(uint32_t width, uint32_t height);
		//~GBufferPBRPass();
		//void InitBuffers(unsigned int width, unsigned int height);
		//void BindTextures();
		//void DrawModel(Model& model, glm::mat4 view, glm::mat4 projection);
		//void BindFramebuffer();
		//void ControlWindow();
		//unsigned int GetGBuffer();
		//int debugPass = 0;
		//bool ambientOcclusionEnabled = true;
		virtual void BindInputs() const override;
		virtual void Draw() const override;
		virtual std::vector<RenderPassInput> GetInputs() const override { return m_Inputs; };

	private:
		std::shared_ptr<Framebuffer> m_Framebuffer;
		std::shared_ptr<Shader> m_Shader;
		std::vector<RenderPassInput> m_Inputs = {};
		//unsigned int gBuffer, gPosition, gNormal, gAlbedo, gMetRoughAO;
		//unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		//unsigned int rboDepth;
		//Shader shader = Shader("Shaders/g_buffer.vert", "Shaders/g_buffer_pbr.frag");
	};
}