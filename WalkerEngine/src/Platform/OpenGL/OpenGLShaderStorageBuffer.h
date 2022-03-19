#pragma once

#include "Renderer/ShaderStorageBuffer.h"

namespace Walker {

	class OpenGLShaderStorageBuffer : public ShaderStorageBuffer
	{
	public:
		OpenGLShaderStorageBuffer(uint32_t size, uint32_t binding);
		virtual ~OpenGLShaderStorageBuffer();

		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;
	private:
		uint32_t m_RendererID = 0;
	};
}