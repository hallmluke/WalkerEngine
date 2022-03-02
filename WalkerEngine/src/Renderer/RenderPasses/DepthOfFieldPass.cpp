#include "walkerpch.h"
#include "DepthOfFieldPass.h"
#include "Renderer/RenderCommand.h"
#include "Core/Input.h"

namespace Walker {

	DepthOfFieldPass::DepthOfFieldPass(uint32_t width, uint32_t height)
	{
		FramebufferSpecification fbSpec;
		fbSpec.Attachments = {
			{ "gColor", FramebufferTextureFormat::RGBA16F, FramebufferTextureType::UNSIGNED_BYTE, FramebufferTextureTarget::TEXTURE_2D }
		};

		fbSpec.Width = width;
		fbSpec.Height = height;
		fbSpec.Samples = 1;
		m_Framebuffer = Framebuffer::Create(fbSpec);
		m_Shader = Shader::Create("Depth Of Field", "Shaders/deferred_shading.vert", "Shaders/depth_of_field.frag");
	}

	void DepthOfFieldPass::BindInputs() const
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

	void DepthOfFieldPass::BindOutput(uint32_t outputSlot, uint32_t inputSlot) const
	{
		m_Framebuffer->BindColorAttachment(outputSlot, inputSlot);
	}

	void DepthOfFieldPass::Draw() const
	{
	}

	RenderPassInput DepthOfFieldPass::GetInput(std::string name) const
	{
		for (auto input : m_Inputs) {
			if (input.Name == name) {
				return input;
			}
		}

		W_CORE_ERROR("No input found with name '{0}'", name);

		return RenderPassInput();
	}

	RenderPassOutput DepthOfFieldPass::GetOutput(std::string name) const
	{
		for (auto output : m_Outputs) {
			if (output.Name == name) {
				return output;
			}
		}

		W_CORE_ERROR("No output found for with name '{0}'", name);

		return RenderPassOutput();
	}

	void DepthOfFieldPass::LinkToInput(std::string inputName, RenderPassOutput output)
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

	void DepthOfFieldPass::DrawScene(Scene& scene) const
	{
		// TODO: Configurable for certain behavior
		//glm::vec2 mouse = Input::GetMousePosition();
		//mouse.x /= m_Framebuffer->GetSpecification().Width;
		//mouse.y /= m_Framebuffer->GetSpecification().Height;

		//RenderCommand::BindDefaultFramebuffer();
		m_Framebuffer->Bind();
		m_Shader->Bind();

		for (auto input : m_Inputs) {
			m_Shader->SetInt(input.Name, input.Slot);
		}

		// TODO: Get from camera
		m_Shader->SetFloat("u_MinDistance", 1.0f);
		m_Shader->SetFloat("u_MaxDistance", 5.0f);
		m_Shader->SetMat4("view", scene.GetCamera()->GetViewMatrix());
		m_Shader->SetMat4("projection", scene.GetCamera()->GetProjectionMatrix());
		//m_Shader->SetVec3("mouse", glm::vec3(mouse.x, 1 - mouse.y, 1));

		BindInputs();

		m_Quad.Draw();
	}

	std::shared_ptr<Framebuffer> DepthOfFieldPass::GetFramebuffer() const
	{
		return m_Framebuffer;
	}

	uint32_t DepthOfFieldPass::GetFinalOutputRendererId() const
	{
		return m_Framebuffer->GetColorAttachmentRendererID();
	}

	void DepthOfFieldPass::Resize(uint32_t width, uint32_t height)
	{
		m_Framebuffer->Resize(width, height);
	}

}