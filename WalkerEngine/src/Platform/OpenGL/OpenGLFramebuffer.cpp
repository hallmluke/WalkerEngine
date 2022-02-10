#include "walkerpch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Walker {

	static const uint32_t s_MaxFramebufferSize = 8192;

	namespace Utils {

		static GLenum TextureTarget(bool multisampled)
		{
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count)
		{
			glGenTextures(count, outID);
			//glCreateTextures(TextureTarget(multisampled), count, outID);
		}

		static void BindTexture(bool multisampled, uint32_t id)
		{
			glBindTexture(TextureTarget(multisampled), id);
		}

		static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, GLenum type, uint32_t width, uint32_t height, int index)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
		}

		static void AttachDepthTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, GLenum type, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				//glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
				//glBindTexture(GL_TEXTURE_2D, id);
				//glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_FLOAT, NULL);
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, NULL);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);

			/*glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, id, 0);*/
		}

		static bool IsDepthFormat(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:  return true;
			case FramebufferTextureFormat::DEPTH32F:         return true;
			}

			return false;
		}

		static GLenum WalkerFBDepthTextureFormatToAttachment(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:  return GL_DEPTH_STENCIL_ATTACHMENT;
			case FramebufferTextureFormat::DEPTH32F:         return GL_DEPTH_ATTACHMENT;
			}

			return 0;
		}

		static GLenum WalkerFBTextureInternalFormatToGL(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case FramebufferTextureFormat::RGBA8:             return GL_RGBA8;
			case FramebufferTextureFormat::RGBA16F:           return GL_RGBA16F;
			case FramebufferTextureFormat::RED_INTEGER:       return GL_R32I;
			case FramebufferTextureFormat::DEPTH24STENCIL8:   return GL_DEPTH24_STENCIL8;
			case FramebufferTextureFormat::DEPTH32F:          return GL_DEPTH_COMPONENT32F;
			}

			//W_CORE_ASSERT(false);
			return 0;
		}

		static GLenum WalkerFBTextureFormatToGL(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case FramebufferTextureFormat::RGBA8:           return GL_RGBA;
			case FramebufferTextureFormat::RGBA16F:         return GL_RGBA;
			case FramebufferTextureFormat::RED_INTEGER:     return GL_RED_INTEGER;
			case FramebufferTextureFormat::DEPTH24STENCIL8: return GL_DEPTH_STENCIL;
			case FramebufferTextureFormat::DEPTH32F:        return GL_DEPTH_COMPONENT;
			}

			//W_CORE_ASSERT(false);
			return 0;
		}

		static GLenum WalkerFBTextureTypeToGL(FramebufferTextureType type)
		{
			switch (type)
			{
			case FramebufferTextureType::FLOAT:             return GL_FLOAT;
			case FramebufferTextureType::UNSIGNED_BYTE:     return GL_UNSIGNED_BYTE;
			case FramebufferTextureType::UNSIGNED_INT_24_8: return GL_UNSIGNED_INT_24_8;
			}
		}

	}

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
		: m_Specification(spec)
	{
		for (auto spec : m_Specification.Attachments.Attachments)
		{
			if (!Utils::IsDepthFormat(spec.TextureFormat))
				m_ColorAttachmentSpecifications.emplace_back(spec);
			else
				m_DepthAttachmentSpecification = spec;
		}

		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void OpenGLFramebuffer::Invalidate()
	{
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);

			m_ColorAttachments.clear();
			m_DepthAttachment = 0;
		}

		glGenFramebuffers(1, &m_RendererID);
		//glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		bool multisample = m_Specification.Samples > 1;

		// Attachments
		if (m_ColorAttachmentSpecifications.size())
		{
			m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
			Utils::CreateTextures(multisample, m_ColorAttachments.data(), m_ColorAttachments.size());

			for (size_t i = 0; i < m_ColorAttachments.size(); i++)
			{
				Utils::BindTexture(multisample, m_ColorAttachments[i]);
				Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, Utils::WalkerFBTextureInternalFormatToGL(m_ColorAttachmentSpecifications[i].TextureFormat),
					Utils::WalkerFBTextureFormatToGL(m_ColorAttachmentSpecifications[i].TextureFormat), Utils::WalkerFBTextureTypeToGL(m_ColorAttachmentSpecifications[i].Type), m_Specification.Width, m_Specification.Height, i);
				/*switch (m_ColorAttachmentSpecifications[i].TextureFormat)
				{
				case FramebufferTextureFormat::RGBA8:
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, i);
					break;
				case FramebufferTextureFormat::RGBA16F:
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA16F, GL_RGBA, m_Specification.Width, m_Specification.Height, i);
					break;
				case FramebufferTextureFormat::RED_INTEGER:
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, i);
					break;
				}*/
			}
		}

		if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
		{
			Utils::CreateTextures(multisample, &m_DepthAttachment, 1);
			Utils::BindTexture(multisample, m_DepthAttachment);
			Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, Utils::WalkerFBTextureInternalFormatToGL(m_DepthAttachmentSpecification.TextureFormat), 
				Utils::WalkerFBTextureFormatToGL(m_DepthAttachmentSpecification.TextureFormat), Utils::WalkerFBTextureTypeToGL(m_DepthAttachmentSpecification.Type), 
				Utils::WalkerFBDepthTextureFormatToAttachment(m_DepthAttachmentSpecification.TextureFormat), m_Specification.Width, m_Specification.Height);
			/*switch (m_DepthAttachmentSpecification.TextureFormat)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
				break;
			case FramebufferTextureFormat::DEPTH32F:
				Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT, m_Specification.Width, m_Specification.Height);
				break;
			}*/
		}

		if (m_ColorAttachments.size() > 1)
		{
			//W_CORE_ASSERT(m_ColorAttachments.size() <= 4);
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColorAttachments.size(), buffers);
		}
		else if (m_ColorAttachments.empty())
		{
			// Only depth-pass
			glDrawBuffer(GL_NONE);
		}

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			W_CORE_ERROR("Error creating framebuffer: {0}", glGetError());
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
		{
			W_CORE_WARN("Attempted to rezize framebuffer to {0}, {1}", width, height);
			return;
		}
		m_Specification.Width = width;
		m_Specification.Height = height;

		Invalidate();
	}

	int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		//W_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size());

		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;

	}

	void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value)
	{
		//W_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size());

		auto& spec = m_ColorAttachmentSpecifications[attachmentIndex];
		// TODO: No good replacement for older OpenGL versions
		//glClearTexImage(m_ColorAttachments[attachmentIndex], 0,
		//	Utils::WalkerFBTextureFormatToGL(spec.TextureFormat), GL_INT, &value);
	}

	void OpenGLFramebuffer::BindColorAttachment(uint32_t index, uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachments[index]);
	}

	void OpenGLFramebuffer::BindColorAttachment(std::string name, uint32_t slot) const
	{
		// TODO: Optimize, store in map?
		int32_t index = -1;
		for (int32_t i = 0; i < m_ColorAttachments.size(); i++) {
			if (m_ColorAttachmentSpecifications[i].Name == name) {
				index = i;
				break;
			}
		}
		if (index != -1) {
			BindColorAttachment(index, slot);
		}
		else {
			W_CORE_ERROR("Attempted to retrieve nonexistent color attachment: {0}", name);
		}
	}

}