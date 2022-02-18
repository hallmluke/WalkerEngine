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
	}

	void BoxBlurPass::BindInputs() const
	{
		for (auto input : m_Inputs) {

			auto it = m_Links.find(input.Name);

			if (it != m_Links.end()) {
				RenderPass* outPass = it->second.RenderPass;
				outPass->BindOutput(it->second.Slot, input.Slot);
			}
			else {
				W_CORE_ERROR("No output linked for input {0}", input.Name);
			}
		}
	}

	void BoxBlurPass::BindOutput(uint32_t outputSlot, uint32_t inputSlot) const
	{
		m_Framebuffer->BindColorAttachment(outputSlot, inputSlot);
	}

	void BoxBlurPass::Draw() const
	{
	}

	RenderPassInput BoxBlurPass::GetInput(std::string name) const
	{
		for (auto input : m_Inputs) {
			if (input.Name == name) {
				return input;
			}
		}

		W_CORE_ERROR("No input found with name '{0}'", name);

		return RenderPassInput();
	}

	RenderPassOutput BoxBlurPass::GetOutput(std::string name) const
	{
		for (auto output : m_Outputs) {
			if (output.Name == name) {
				return output;
			}
		}

		W_CORE_ERROR("No output found for with name '{0}'", name);

		return RenderPassOutput();
	}

	void BoxBlurPass::LinkToInput(std::string inputName, RenderPassOutput output)
	{
		RenderPassInput passInput;
		bool found = false;

		for (auto input : m_Inputs) {
			if (input.Name == inputName) {
				passInput = input;
				found = true;
				break;
			}
		}

		if (found) {
			m_Links[inputName] = output;
		}
		else {
			W_CORE_ERROR("No input found with name {0} in DepthOfFieldPass", inputName);
		}
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
}