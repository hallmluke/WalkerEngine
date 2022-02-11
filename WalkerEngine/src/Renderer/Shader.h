#pragma once

#include <string>

#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include "Scene/DirectionalLight.h"
#include "Scene/Camera.h"

namespace Walker {

	class Shader {
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetBool(const std::string& name, bool value) = 0;
		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetMat4(const std::string& name, glm::mat4 value) = 0;
		virtual void SetVec3(const std::string& name, float x, float y, float z) = 0;
		virtual void SetVec3(const std::string& name, glm::vec3 value) = 0;
		//virtual void SetPointLightProperties(PointLight light) = 0;
		//virtual void SetPointLightProperties(std::vector<PointLight*> lights) = 0;
		virtual void SetDirectionalLightProperties(DirectionalLight light, Camera camera) = 0;
	
		virtual const std::string& GetName() const = 0;

		static std::shared_ptr<Shader> Create(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = "");

	};

	class ShaderLibrary
	{
	public:
		void Add(const std::string& name, const std::shared_ptr<Shader>& shader);
		void Add(const std::shared_ptr<Shader>& shader);
		std::shared_ptr<Shader> Load(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = "");

		std::shared_ptr<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
	};
}