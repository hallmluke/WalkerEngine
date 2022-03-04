#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Renderer/ComputeShader.h"

namespace Walker {

	class OpenGLComputeShader : public ComputeShader {
	public:
		OpenGLComputeShader(const std::string& name, const std::string& path);
		~OpenGLComputeShader();

		virtual const std::string& GetName() const override { return m_Name; };

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void Dispatch(uint32_t numGroupsX, uint32_t numGroupsY, uint32_t numGroupsZ) override;
		virtual void Barrier() override;

		virtual void SetBool(const std::string& name, bool value) override;
		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetMat4(const std::string& name, glm::mat4 value) override;
		virtual void SetVec3(const std::string& name, float x, float y, float z) override;
		virtual void SetVec3(const std::string& name, glm::vec3 value) override;


	private:
		unsigned int m_Id;
		mutable std::unordered_map<std::string, GLint> m_UniformLocationCache;
		std::string m_Name;

		void CheckCompileErrors(GLuint shader, std::string type);
		GLint GetUniformLocation(const std::string& name) const;
	};
}