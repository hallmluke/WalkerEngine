#pragma once

namespace Walker {

	class ShaderStorageBuffer {
	public:
		virtual ~ShaderStorageBuffer() {}
		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;

		static std::shared_ptr<ShaderStorageBuffer> Create(uint32_t size, uint32_t binding);
	};
}