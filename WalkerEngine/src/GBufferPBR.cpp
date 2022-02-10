#include "walkerpch.h"
#include "GBufferPBR.h"
#include <iostream>
#include "imgui/imgui.h"

#include "Renderer/Framebuffer.h"
#include "Renderer/Shader.h"
#include "Renderer/RenderCommand.h"

namespace Walker {

    /*GBufferPBR::GBufferPBR(unsigned int width, unsigned int height)
    {
        InitBuffers(width, height);
    }

    void GBufferPBR::InitBuffers(unsigned int width, unsigned int height)
    {
        FramebufferSpecification fbSpec;
        fbSpec.Attachments = { { "gPosition", FramebufferTextureFormat::RGBA16F, FramebufferTextureType::FLOAT }, { "gNormal", FramebufferTextureFormat::RGBA16F, FramebufferTextureType::FLOAT } };
            // configure g-buffer framebuffer
            // ------------------------------
            glGenFramebuffers(1, &gBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        // position color buffer
        glGenTextures(1, &gPosition);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexImage2D()
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
        // normal color buffer
        glGenTextures(1, &gNormal);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
        // albedo color buffer
        glGenTextures(1, &gAlbedo);
        glBindTexture(GL_TEXTURE_2D, gAlbedo);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);
        // Metallic, Roughness, AO buffer
        glGenTextures(1, &gMetRoughAO);
        glBindTexture(GL_TEXTURE_2D, gMetRoughAO);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gMetRoughAO, 0);
        // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
        glDrawBuffers(4, attachments);
        // create and attach depth buffer (renderbuffer)
        glGenRenderbuffers(1, &rboDepth);
        glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
        // finally check if framebuffer is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "Framebuffer not complete!" << std::endl;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    void GBufferPBR::BindTextures()
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gAlbedo);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, gMetRoughAO);
    }

    void GBufferPBR::DrawModel(Model& model, glm::mat4 view, glm::mat4 projection)
    {
        shader.use();
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        model.Draw(shader);
    }

    void GBufferPBR::BindFramebuffer()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void GBufferPBR::ControlWindow()
    {
        if (ImGui::Begin("G Buffer")) {
            ImGui::SliderInt("Debug Pass", &debugPass, 0, 6);
            ImGui::Checkbox("Ambient Occlusion", &ambientOcclusionEnabled);
        }
        ImGui::End();
    }

    unsigned int GBufferPBR::GetGBuffer()
    {
        return gBuffer;
    }*/

    GBufferPBRPass::GBufferPBRPass(uint32_t width, uint32_t height) {
        FramebufferSpecification fbSpec;
        fbSpec.Attachments = { 
            { "gPosition", FramebufferTextureFormat::RGBA16F, FramebufferTextureType::FLOAT }, 
            { "gNormal", FramebufferTextureFormat::RGBA16F, FramebufferTextureType::FLOAT },
            { "gAlbedo", FramebufferTextureFormat::RGBA16F, FramebufferTextureType::UNSIGNED_BYTE },
            { "gMetRoughAO", FramebufferTextureFormat::RGBA16F, FramebufferTextureType::UNSIGNED_BYTE },
            { "Depth", FramebufferTextureFormat::DEPTH32F, FramebufferTextureType::FLOAT }
        };
        fbSpec.Width = width;
        fbSpec.Height = height;
        fbSpec.Samples = 1;
        m_Framebuffer = Framebuffer::Create(fbSpec);

        // TODO: Use shader library?
        //m_Shader = Shader::Create("GBufferPBR", "Shaders/g_buffer.vert", "Shaders/g_buffer_pbr.frag");
        m_Shader = Shader::Create("GBufferPBR", "Shaders/g_buffer.vert", "Shaders/test.frag");
    }

    void GBufferPBRPass::Draw() const {
        m_Framebuffer->Bind();
        // Draw some objects
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

    void GBufferPBRPass::BindTextures()
    {
        m_Framebuffer->BindColorAttachment(0, 0);
        m_Framebuffer->BindColorAttachment(1, 1);
        m_Framebuffer->BindColorAttachment(2, 2);
    }

    void GBufferPBRPass::BindInputs() const {
        // No inputs
        return;
    }

}