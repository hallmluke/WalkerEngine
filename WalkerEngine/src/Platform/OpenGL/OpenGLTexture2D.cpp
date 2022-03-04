#include "walkerpch.h"
#include "Platform/OpenGL/OpenGLTexture2D.h"

#include <stb_image.h>

namespace Walker {

	namespace Utils {

		static GLenum WalkerTextureInternalFormatToGL(TextureFormat format)
		{
			switch (format)
			{
			case TextureFormat::RGBA8:             return GL_RGBA8;
			case TextureFormat::RGBA16F:           return GL_RGBA16F;
			case TextureFormat::RGBA32F:           return GL_RGBA32F;
			case TextureFormat::RED_INTEGER:       return GL_R32I;
			case TextureFormat::DEPTH24STENCIL8:   return GL_DEPTH24_STENCIL8;
			case TextureFormat::DEPTH32F:          return GL_DEPTH_COMPONENT32F;
			}

			//W_CORE_ASSERT(false);
			return 0;
		}

		static GLenum WalkerTextureFormatToGL(TextureFormat format)
		{
			switch (format)
			{
			case TextureFormat::RGBA8:           return GL_RGBA;
			case TextureFormat::RGBA16F:         return GL_RGBA;
			case TextureFormat::RGBA32F:         return GL_RGBA;
			case TextureFormat::RED_INTEGER:     return GL_RED_INTEGER;
			case TextureFormat::DEPTH24STENCIL8: return GL_DEPTH_STENCIL;
			case TextureFormat::DEPTH32F:        return GL_DEPTH_COMPONENT;
			}

			//W_CORE_ASSERT(false);
			return 0;
		}

		static GLenum WalkerTextureTypeToGL(TextureType type)
		{
			switch (type)
			{
			case TextureType::FLOAT:             return GL_FLOAT;
			case TextureType::UNSIGNED_BYTE:     return GL_UNSIGNED_BYTE;
			case TextureType::UNSIGNED_INT_24_8: return GL_UNSIGNED_INT_24_8;
			}

			return 0;
		}

		static GLenum WalkerTextureWrapTypeToGL(TextureWrapType type) {
			switch (type) {
			case TextureWrapType::CLAMP_BORDER:      return GL_CLAMP_TO_BORDER;
			case TextureWrapType::CLAMP_EDGE:        return GL_CLAMP_TO_EDGE;
			case TextureWrapType::MIRRORED_REPEAT:   return GL_MIRRORED_REPEAT;
			case TextureWrapType::MIRROR_CLAMP_EDGE: return GL_MIRROR_CLAMP_TO_EDGE;
			case TextureWrapType::REPEAT:            return GL_REPEAT;
			}

			return 0;
		}

		static GLenum WalkerTextureFilterTypeToGL(TextureFilterType type) {
			switch (type) {
			case TextureFilterType::LINEAR:                 return GL_LINEAR;
			case TextureFilterType::LINEAR_MIPMAP_LINEAR:   return GL_LINEAR_MIPMAP_LINEAR;
			case TextureFilterType::LINEAR_MIPMAP_NEAREST:  return GL_LINEAR_MIPMAP_NEAREST;
			case TextureFilterType::NEAREST:                return GL_NEAREST;
			case TextureFilterType::NEAREST_MIMAP_LINEAR:   return GL_NEAREST_MIPMAP_LINEAR;
			case TextureFilterType::NEAREST_MIPMAP_NEAREST: return GL_NEAREST_MIPMAP_NEAREST;
			}

			return 0;
		}

		static GLenum WalkerTextureAccessToGL(TextureAccess access) {
			switch (access) {
			case TextureAccess::READ_ONLY: return GL_READ_ONLY;
			case TextureAccess::READ_WRITE: return GL_READ_WRITE;
			case TextureAccess::WRITE_ONLY: return GL_WRITE_ONLY;
			}
		}
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height)
	{
		//W_PROFILE_FUNCTION();

		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glGenTextures(1, &m_RendererID);
		//glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		//glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		/*glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path)
	{
		//W_PROFILE_FUNCTION();

		int width, height, channels;
		//stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			//W_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}

		if (data)
		{
			m_IsLoaded = true;

			m_Width = width;
			m_Height = height;

			GLenum internalFormat = 0, dataFormat = 0;
			if (channels == 4)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (channels == 3)
			{
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}

			m_InternalFormat = internalFormat;
			m_DataFormat = dataFormat;

			//W_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

			glGenTextures(1, &m_RendererID);
			//glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			//glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);
			glBindTexture(GL_TEXTURE_2D, m_RendererID);
			glTexImage2D(GL_TEXTURE_2D, 0, dataFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
			//glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, internalFormat, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			/*glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);*/

			stbi_image_free(data);
		}
		else
		{
			m_IsLoaded = false;
			W_CORE_ERROR("Failed to load texture: {0}", path);
		}
	}

	OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& spec)
	{
		m_Width = spec.Width;
		m_Height = spec.Height;
		m_InternalFormat = Utils::WalkerTextureInternalFormatToGL(spec.TextureFormat);
		m_DataFormat = Utils::WalkerTextureFormatToGL(spec.TextureFormat);

		glGenTextures(1, &m_RendererID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Utils::WalkerTextureWrapTypeToGL(spec.WrapS));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Utils::WalkerTextureWrapTypeToGL(spec.WrapT));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Utils::WalkerTextureFilterTypeToGL(spec.MagFilter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Utils::WalkerTextureFilterTypeToGL(spec.MinFilter));

		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, Utils::WalkerTextureTypeToGL(spec.Type), NULL);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		//W_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		//W_PROFILE_FUNCTION();

		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		//W_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexImage2D(GL_TEXTURE_2D, 0, m_DataFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
		//glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		//W_PROFILE_FUNCTION();
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glActiveTexture(GL_TEXTURE0);
		//glBindTextureUnit(slot, m_RendererID);
	}
	void OpenGLTexture2D::BindImage(uint32_t slot, TextureAccess access) const
	{
		// TODO: Paramters to pass through
		glBindImageTexture(slot, m_RendererID, 0, GL_FALSE, 0, Utils::WalkerTextureAccessToGL(access), m_InternalFormat);
		//W_CORE_ERROR("Error in image binding {0}", glGetError());
	}
}