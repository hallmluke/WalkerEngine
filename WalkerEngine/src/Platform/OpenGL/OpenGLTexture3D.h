#pragma once

#include "Renderer/Texture.h"

#include <glad/glad.h>

namespace Walker {

	class OpenGLTexture3D : public Texture3D
	{
	public:
		OpenGLTexture3D(uint32_t width, uint32_t height, uint32_t depth);
		OpenGLTexture3D(const std::string& path);
		OpenGLTexture3D(const TextureSpecification& spec);
		virtual ~OpenGLTexture3D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetDepth() const override { return m_Depth; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;
		virtual void BindImage(uint32_t slot, TextureAccess access) const override;

		virtual bool IsLoaded() const override { return m_IsLoaded; }
		virtual void GenerateMipMaps() override;

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((OpenGLTexture3D&)other).m_RendererID;
		}
	private:
		std::string m_Path;
		bool m_IsLoaded = false;
		uint32_t m_Width, m_Height, m_Depth;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};

}