#include "walkerpch.h"
#include "OpenGLTextureCubemap.h"

#include "stb_image.h"

namespace Walker {

	OpenGLTextureCubeMap::OpenGLTextureCubeMap(const std::string& dir)
	{
		std::vector<std::string> faces{ "right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg" };

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
		int width, height, channels;
		for (unsigned int i = 0; i < faces.size(); i++)
		{
			stbi_uc* data = stbi_load((dir + "/" + faces[i]).c_str(), &width, &height,
				&channels, 0);
			m_IsLoaded = true;

			if (data)
			{
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

				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, dataFormat,
					width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				m_IsLoaded = false;
				W_CORE_ERROR("Cubemap failed to load at path: {0}", dir + faces[i]);
				stbi_image_free(data);
				return;
			}
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,
			GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,
			GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,
			GL_CLAMP_TO_EDGE);
	}

	OpenGLTextureCubeMap::~OpenGLTextureCubeMap()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTextureCubeMap::SetData(void* data, uint32_t size)
	{
		// TODO: Implement for cubemap
	}

	void OpenGLTextureCubeMap::Bind(uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
		glActiveTexture(GL_TEXTURE0);
	}
	void OpenGLTextureCubeMap::BindImage(uint32_t slot) const
	{
	}
}