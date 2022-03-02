#include "walkerpch.h"
#include "EditorPass.h"
#include "Renderer/RenderCommand.h"


namespace Walker {

	EditorPass::EditorPass(uint32_t width, uint32_t height)
	{
		FramebufferSpecification fbSpec;
		fbSpec.Attachments = {
			{"gColor", FramebufferTextureFormat::RGBA8, FramebufferTextureType::UNSIGNED_BYTE, FramebufferTextureTarget::TEXTURE_2D },
			{"gEntities", FramebufferTextureFormat::RED_INTEGER, FramebufferTextureType::UNSIGNED_BYTE, FramebufferTextureTarget::TEXTURE_2D },
			{ "Depth", FramebufferTextureFormat::DEPTH24STENCIL8, FramebufferTextureType::UNSIGNED_INT_24_8, FramebufferTextureTarget::TEXTURE_2D }
		};
		fbSpec.Width = width;
		fbSpec.Height = height;
		fbSpec.Samples = 1;

		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_Shader = Shader::Create("Editor", "Shaders/editor.vert", "Shaders/editor.frag");

	}

	void EditorPass::BindInputs() const
	{
		// No inputs
	}

	void EditorPass::BindOutput(uint32_t outputSlot, uint32_t inputSlot) const
	{
		m_Framebuffer->BindColorAttachment(outputSlot, inputSlot);
	}

	void EditorPass::Draw() const
	{
	}

	RenderPassInput EditorPass::GetInput(std::string name) const
	{
		// No inputs
		return RenderPassInput();
	}

	RenderPassOutput EditorPass::GetOutput(std::string name) const
	{
		for (auto output : m_Outputs) {
			if (output.Name == name) {
				return output;
			}
		}

		W_CORE_ERROR("No output found with name '{0}'", name);

		return RenderPassOutput();
	}

	void EditorPass::LinkToInput(std::string inputName, RenderPassOutput output)
	{
		// No inputs
	}

	void EditorPass::DrawScene(Scene& scene) const
	{
		m_Framebuffer->Bind();
		RenderCommand::Clear();
		m_Shader->Bind();
		scene.DrawEditor(m_Shader);
	}

	std::shared_ptr<Framebuffer> EditorPass::GetFramebuffer() const
	{
		return m_Framebuffer;
	}

	void EditorPass::BindTextures()
	{
	}

	uint32_t EditorPass::GetFinalOutputRendererId() const
	{
		return m_Framebuffer->GetColorAttachmentRendererID();
	}

	void EditorPass::Resize(uint32_t width, uint32_t height)
	{
		m_Framebuffer->Resize(width, height);
	}

	void EditorPass::ReadPixel(uint32_t index, int32_t mouseX, int32_t mouseY)
	{
		m_Framebuffer->ReadPixel(index, mouseX, mouseY);
	}

}