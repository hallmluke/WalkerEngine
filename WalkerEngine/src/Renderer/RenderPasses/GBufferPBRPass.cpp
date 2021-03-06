#include "walkerpch.h"
#include "GBufferPBRPass.h"
#include <iostream>
#include "imgui/imgui.h"

#include "Renderer/Framebuffer.h"
#include "Renderer/Shader.h"
#include "Renderer/RenderCommand.h"

namespace Walker {

    GBufferPBRPass::GBufferPBRPass(uint32_t width, uint32_t height) {
        FramebufferSpecification fbSpec;
        fbSpec.Attachments = { 
            { "gPosition", FramebufferTextureFormat::RGBA16F, FramebufferTextureType::FLOAT, FramebufferTextureTarget::TEXTURE_2D }, 
            { "gNormal", FramebufferTextureFormat::RGBA16F, FramebufferTextureType::FLOAT, FramebufferTextureTarget::TEXTURE_2D },
            { "gAlbedo", FramebufferTextureFormat::RGBA16F, FramebufferTextureType::UNSIGNED_BYTE, FramebufferTextureTarget::TEXTURE_2D },
            { "gMetRoughAO", FramebufferTextureFormat::RGBA16F, FramebufferTextureType::UNSIGNED_BYTE, FramebufferTextureTarget::TEXTURE_2D },
            //{ "Depth", FramebufferTextureFormat::DEPTH32F, FramebufferTextureType::FLOAT, FramebufferTextureTarget::TEXTURE_2D }
            { "Depth", FramebufferTextureFormat::DEPTH24STENCIL8, FramebufferTextureType::UNSIGNED_INT_24_8, FramebufferTextureTarget::TEXTURE_2D }
        };
        fbSpec.Width = width;
        fbSpec.Height = height;
        
        fbSpec.Samples = 1;
        m_Framebuffer = Framebuffer::Create(fbSpec);

        // TODO: Use shader library?
        //m_Shader = Shader::Create("GBufferPBR", "Shaders/g_buffer.vert", "Shaders/g_buffer_pbr.frag");
        m_Shader = Shader::Create("GBufferPBR", "Shaders/g_buffer.vert", "Shaders/test.frag");
    }

    RenderPassInput GBufferPBRPass::GetInput(std::string name) const
    {
        // TODO: Don't make pure virtual, default implementation?
        return RenderPassInput();
    }

    RenderPassOutput GBufferPBRPass::GetOutput(std::string name) const
    {
        for (auto output : m_Outputs) {
            if (output.Name == name) {
                return output;
            }
        }
        
        W_CORE_ERROR("No output found for with name '{0}'", name);

        return RenderPassOutput();
    }

    void GBufferPBRPass::LinkToInput(std::string inputName, RenderPassOutput output)
    {
        // TODO: All of these functions probably don't need to be pure virtual...
    }

    void GBufferPBRPass::DrawModel(Model& model, glm::mat4 view, glm::mat4 projection) const
    {
        m_Framebuffer->Bind();
        RenderCommand::Clear();
        m_Shader->Bind();
        m_Shader->SetMat4("view", view);
        m_Shader->SetMat4("projection", projection);
        model.Draw(m_Shader);
    }

	void GBufferPBRPass::DrawScene(Scene& scene) const
	{
        m_Framebuffer->Bind();
        RenderCommand::Clear();
        m_Shader->Bind();
        scene.DrawMeshes(m_Shader);
	}

    std::shared_ptr<Framebuffer> GBufferPBRPass::GetFramebuffer() const
    {
        return m_Framebuffer;
    }

    void GBufferPBRPass::BindTextures()
    {
        m_Framebuffer->BindColorAttachment(0, 0);
        m_Framebuffer->BindColorAttachment(1, 1);
        m_Framebuffer->BindColorAttachment(2, 2);
        m_Framebuffer->BindColorAttachment(3, 3);
    }

	uint32_t GBufferPBRPass::GetFinalOutputRendererId() const
	{
        return m_Framebuffer->GetColorAttachmentRendererID();
	}

	void GBufferPBRPass::Resize(uint32_t width, uint32_t height)
	{
        m_Framebuffer->Resize(width, height);
	}

    void GBufferPBRPass::BindInputs() const {
        // No inputs
        return;
    }

    void GBufferPBRPass::BindOutput(uint32_t outputSlot, uint32_t inputSlot) const {
        m_Framebuffer->BindColorAttachment(outputSlot, inputSlot);
    }

    void GBufferPBRPass::BindOutputImage(uint32_t outputSlot, uint32_t inputSlot) const
    {
        m_Framebuffer->BindColorAttachmentImage(outputSlot, inputSlot);
    }

}