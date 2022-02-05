#pragma once

namespace Walker {

	enum class FramebufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8,
		RGBA16F,
		RED_INTEGER,

		// Depth/stencil
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8
	};

	enum class FramebufferTextureType
	{
		None = 0,
		UNSIGNED_BYTE,
		FLOAT
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(std::string name, FramebufferTextureFormat format, FramebufferTextureType type)
			: Name(name), TextureFormat(format), Type(type) {}

		std::string Name;
		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
		FramebufferTextureType Type = FramebufferTextureType::None;
		// TODO: filtering/wrap
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	struct FramebufferSpecification
	{
		uint32_t Width = 0, Height = 0;
		FramebufferAttachmentSpecification Attachments;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;
		virtual void BindColorAttachment(uint32_t index = 0, uint32_t slot = 0) const = 0;
		virtual void BindColorAttachment(std::string name, uint32_t slot = 0) const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static std::shared_ptr<Framebuffer> Create(const FramebufferSpecification& spec);

		// Temporary
		virtual uint32_t GetId() const = 0;
	};


}