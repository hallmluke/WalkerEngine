 #pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <GLM/glm.hpp>

namespace Walker {

	class ComputeShader {
	public:
		virtual ~ComputeShader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void Dispatch(uint32_t numGroupsX, uint32_t numGroupsY, uint32_t numGroupsZ) = 0;
		virtual void Barrier() = 0;

		virtual void SetBool(const std::string& name, bool value) = 0;
		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetMat4(const std::string& name, glm::mat4 value) = 0;
		virtual void SetVec3(const std::string& name, float x, float y, float z) = 0;
		virtual void SetVec3(const std::string& name, glm::vec3 value) = 0;

		virtual const std::string& GetName() const = 0;

		static std::shared_ptr<ComputeShader> Create(const std::string& name, const std::string& path);

	};

}