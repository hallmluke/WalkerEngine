#pragma once

#include "Renderer/Framebuffer.h"

namespace Walker {

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { /*W_CORE_ASSERT(index < m_ColorAttachments.size());*/ return m_ColorAttachments[index]; }
		virtual void BindColorAttachment(uint32_t index = 0, uint32_t slot = 0) const override;
		virtual void BindColorAttachment(std::string name, uint32_t slot = 0) const override;
		virtual void BindDepthAttachment(uint32_t slot = 0) const override;

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }

		virtual uint32_t GetId() const override { return m_RendererID; }
	private:
		uint32_t m_RendererID = 0;
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = { "Depth", FramebufferTextureFormat::DEPTH32F, FramebufferTextureType::FLOAT, FramebufferTextureTarget::TEXTURE_2D };

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
	};

}