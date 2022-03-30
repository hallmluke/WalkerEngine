#include "walkerpch.h"
#include "BoxBlurPass.h"
#include "Renderer/RenderCommand.h"

namespace Walker {

	BoxBlurPass::BoxBlurPass(uint32_t width, uint32_t height)
	{
		FramebufferSpecification fbSpec;
		fbSpec.Attachments = {
			{ "gColor", FramebufferTextureFormat::RGBA16F, FramebufferTextureType::UNSIGNED_BYTE, FramebufferTextureTarget::TEXTURE_2D }
		};

		fbSpec.Width = width;
		fbSpec.Height = height; 
		fbSpec.Samples = 1;
		m_Framebuffer = Framebuffer::Create(fbSpec);
		m_Shader = Shader::Create("Box Blur", "Shaders/deferred_shading.vert", "Shaders/box_blur.frag");

		m_Inputs = { {"u_ColorTexture", 0, this } };
		m_Outputs = { {"gColor", 0, this } };
	}

	void BoxBlurPass::BindOutput(uint32_t outputSlot, uint32_t inputSlot) const
	{
		m_Framebuffer->BindColorAttachment(outputSlot, inputSlot);
	}

	void BoxBlurPass::BindOutputImage(uint32_t outputSlot, uint32_t inputSlot) const
	{
	}

	void BoxBlurPass::DrawScene(Scene& scene) const
	{
		//RenderCommand::BindDefaultFramebuffer();
		m_Framebuffer->Bind();
		RenderCommand::Clear();
		m_Shader->Bind();

		for (auto input : m_Inputs) {
			m_Shader->SetInt(input.Name, input.Slot);
		}

		BindInputs();

		m_Quad.Draw();
	}

	std::shared_ptr<Framebuffer> BoxBlurPass::GetFramebuffer() const
	{
		return m_Framebuffer;
	}

	uint32_t BoxBlurPass::GetFinalOutputRendererId() const
	{
		return m_Framebuffer->GetColorAttachmentRendererID();
	}

	void BoxBlurPass::Resize(uint32_t width, uint32_t height)
	{
		m_Framebuffer->Resize(width, height);
	}
}