#pragma once

#include "Renderer/Texture.h"

#include <glad/glad.h>

namespace Walker {

	namespace Utils {

		static GLenum WalkerTextureInternalFormatToGL(TextureFormat format)
		{
			switch (format)
			{
			case TextureFormat::RGBA8:             return GL_RGBA8;
			case TextureFormat::RGBA16:            return GL_RGBA16;
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
			case TextureFormat::RGBA16:          return GL_RGBA;
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

			return 0;
		}
	}
}